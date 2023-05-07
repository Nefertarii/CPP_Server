#include "Include/Eventloop/timequeue.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Logger/logger.h"
#include "Include/Timer/clock.h"
#include <sys/timerfd.h>
#include <unistd.h>
#include <cstring>

using namespace wasi;
using namespace wasi::loop;

int TimerQueue::CreateTimerfd() {
    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0) { LOG_ERROR("Failed create timerfd."); }
    return timerfd;
}

void TimerQueue::ReadTimerfd(int timerfd, timer::TimeStamp now) {
    long remaining;
    ssize_t ret = read(timerfd, &remaining, sizeof(remaining));
    if (ret != sizeof(remaining)) {
        LOG_ERROR("Handle read " + std::to_string(ret) + " bytes, Should be 8.");
    }
}

void TimerQueue::ResetTimerfd(int timerfd, timer::TimeStamp expiration) {
    itimerspec new_value, old_value;
    memset(&new_value, 0, sizeof(new_value));
    memset(&old_value, 0, sizeof(old_value));
    new_value.it_value = TimeFromNow(expiration);
    int ret            = timerfd_settime(timerfd, 0, &new_value, &old_value);
    if (ret) { LOG_WARN("timerfd not reset."); }
}

timespec TimerQueue::TimeFromNow(timer::TimeStamp when) {
    long milliseconds = when.GetTime() - timer::Clock::NowtimeMs();
    if (milliseconds < 100) { milliseconds = 100; }
    timespec ts;
    ts.tv_sec  = static_cast<time_t>(milliseconds / timer::TimeStamp::millisecond_per_second);
    ts.tv_nsec = static_cast<long>(milliseconds * 1000);
    return ts;
}

void TimerQueue::HandleRead() {
    if (eventloop_->IsInLoopThread()) {
        timer::TimeStamp now(timer::Clock::NowtimeMs());
        ReadTimerfd(timerfd_, now);
        std::vector<Expired> expired = GetExpired(now);
        for (std::vector<Expired>::iterator it = expired.begin(); it != expired.end(); ++it) {
            it->second->Run();
        }
        Reset(expired, now);
    }
}

std::vector<Expired> TimerQueue::GetExpired(timer::TimeStamp now) {
    if (timers_.size() != active_timers_.size()) {
        LOG_WARN("timers size not eaqul active timers size");
    }
    std::vector<Expired> expired;
    Expired sentry         = std::make_pair(now, reinterpret_cast<timer::Timer*>(UINTPTR_MAX));
    TimerList::iterator it = timers_.lower_bound(sentry); // end
    std::copy(timers_.begin(), it, back_inserter(expired));
    timers_.erase(timers_.begin(), it);
    for (const Expired& it : expired) {
        ActiveTimer timer(it.second, it.second->Sequence());
        active_timers_.erase(timer);
    }
    return expired;
}

bool TimerQueue::Insert(timer::Timer* timer) {
    bool earliest_changed = false;
    if (eventloop_->IsInLoopThread()) {
        if (timers_.size() != active_timers_.size()) {
            LOG_WARN("timers size not eaqul active timers size");
        }
        timer::TimeStamp when  = timer->Expiration();
        TimerList::iterator it = timers_.begin();
        if (it == timers_.end() || when < it->first) {
            earliest_changed = true;
        }
        timers_.insert(Expired(when, timer));
        active_timers_.insert(ActiveTimer(timer, timer->Sequence()));
        if (timers_.size() != active_timers_.size()) {
            LOG_ERROR("timers insert fail");
        }
    }
    return earliest_changed;
}

void TimerQueue::AddTimerInLoop(timer::Timer* timer) {
    if (eventloop_->IsInLoopThread()) {
        bool earliest_changed = Insert(timer);
        if (earliest_changed) { ResetTimerfd(timerfd_, timer->Expiration()); }
    }
}

void TimerQueue::Reset(const std::vector<Expired>& expired, timer::TimeStamp now) {
    timer::TimeStamp next_expire;
    for (std::vector<Expired>::const_iterator it = expired.begin(); it != expired.end(); ++it) {
        if (it->second->Repeat()) {
            it->second->Restart(now);
            Insert(it->second);
        } else {
            delete it->second;
        }
    }
    if (!timers_.empty()) {
        next_expire = timers_.begin()->second->Expiration();
    }
    if (next_expire.Valid()) {
        ResetTimerfd(timerfd_, next_expire);
    }
}

TimerQueue::TimerQueue(EventLoop* eventloop) :
    timerfd_(CreateTimerfd()),
    eventloop_(eventloop),
    timerfd_channel_(eventloop_, timerfd_) {
    timerfd_channel_.SetReadCallback(std::bind(&TimerQueue::HandleRead, this));
    timerfd_channel_.EnableReading();
}

void TimerQueue::AddTimer(std::function<void()> callback, timer::TimeStamp when, double interval) {
    timer::Timer* timer_ = new timer::Timer(callback, when, interval);
    eventloop_->RunInLoop(std::bind(&TimerQueue::AddTimerInLoop, this, timer_));
    // return Time::TimerId(timer_);
}

TimerQueue::~TimerQueue() {
    close(timerfd_);
    for (const Expired& timer : timers_) {
        delete timer.second;
    }
}