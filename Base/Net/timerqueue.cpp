#include "timerqueue.h"
#include "../Log/logging.h"
#include "eventloop.h"
#include "Timer/clock.h"
#include "Timer/timer.h"
#include "Timer/timerid.h"
#include "Timer/timestamp.h"
#include <cassert>
#include <string.h>
#include <sys/timerfd.h>

using namespace Wasi::Time;

int Create_timerfd() {
    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0) { LOG_ERROR("Failed create timerfd."); }
    return timerfd;
}

timespec Time_from_now(TimeStamp when) {
    long microseconds = when.Microseconds_since_epoch() - Clock::Nowtime_us();
    if (microseconds < 100) { microseconds = 100; }
    timespec ts;
    ts.tv_sec  = static_cast<time_t>(microseconds / TimeStamp::microseconds_per_second);
    ts.tv_nsec = static_cast<long>((microseconds % TimeStamp::microseconds_per_second) * 1000);
    return ts;
}

void Read_timerfd(int timerfd_, TimeStamp now) {
    long remaining;
    ssize_t ret     = read(timerfd_, &remaining, sizeof(remaining));
    std::string msg = "Handle read " + std::to_string(remaining)
                      + " at:" + std::to_string(now.Microseconds_since_epoch());
    LOG_DEBUG(msg);
    if (ret != sizeof(remaining)) {
        msg = "Handle read " + std::to_string(ret) + " bytes, Should be 8.";
        LOG_ERROR(msg);
    }
}

void Reset_timerfd(int timerfd_, TimeStamp expiration) {
    itimerspec new_value, old_value;
    memset(&new_value, 0, sizeof(new_value));
    memset(&old_value, 0, sizeof(old_value));
    new_value.it_value = Time_from_now(expiration);
    int ret            = timerfd_settime(timerfd_, 0, &new_value, &old_value);
    if (ret) { LOG_WARN("timerfd not reset."); }
}

void TimerQueue::Handle_read() {
    loop->Assert_in_loop_thread();
    TimeStamp now(Clock::Nowtime_us());
    Read_timerfd(timerfd, now);
    std::vector<Entry> expired = Get_expired(now);
    for (std::vector<Entry>::iterator it = expired.begin(); it != expired.end(); ++it) {
        it->second->Run();
    }
    Reset(expired, now);
}

std::vector<Entry> TimerQueue::Get_expired(TimeStamp now) {
    assert(timers.size() == active_timers.size());
    std::vector<Entry> expired;
    Entry sentry           = std::make_pair(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    TimerList::iterator it = timers.lower_bound(sentry); // end
    assert(it == timers.end() || now < it->first);
    std::copy(timers.begin(), it, back_inserter(expired));
    timers.erase(timers.begin(), it);
    for (const Entry& it : expired) {
        ActiveTimer timer(it.second, it.second->Sequence());
        size_t n = active_timers.erase(timer);
        assert(n == 1);
        (void)n;
    }
    assert(timers.size() == active_timers.size());
    return expired;
}

void TimerQueue::Reset(const std::vector<Entry>& expired, TimeStamp now) {
    TimeStamp next_expire;
    for (std::vector<Entry>::const_iterator it = expired.begin(); it != expired.end(); ++it) {
        if (it->second->Repeat()) {
            it->second->Restart(now);
            Insert(it->second);
        } else {
            delete it->second;
        }
    }
    if (!timers.empty()) {
        next_expire = timers.begin()->second->Expiration();
    }
    if (next_expire.Valid()) {
        Reset_timerfd(timerfd, next_expire);
    }
}

bool TimerQueue::Insert(Timer* timer) {
    loop->Assert_in_loop_thread();
    assert(timers.size() == active_timers.size());
    bool earlist_changed   = false;
    TimeStamp when         = timer->Expiration();
    TimerList::iterator it = timers.begin();
    if (it == timers.end() || when < it->first) {
        earlist_changed = true;
    }
    {
        std::pair<TimerList::iterator, bool> result =
            timers.insert(Entry(when, timer));
        assert(result.second);
        (void)result;
    }
    {
        std::pair<ActiveTimerSet::iterator, bool> result =
            active_timers.insert(ActiveTimer(timer, timer->Sequence()));
        assert(result.second);
        (void)result;
    }
    assert(timers.size() == active_timers.size());
    return earlist_changed;
}

void TimerQueue::Add_timer_in_loop(Timer* timer) {
    loop->Assert_in_loop_thread();
    bool earliest_changed = Insert(timer);
    if (earliest_changed) { Reset_timerfd(timerfd, timer->Expiration()); }
}

TimerQueue::TimerQueue(Poll::EventLoop* loop_) :
    loop(loop_),
    timerfd(Create_timerfd()),
    timerfd_channel(loop, timerfd),
    timers() {
    timerfd_channel.Set_read_callback(std::bind(&TimerQueue::Handle_read, this));
    timerfd_channel.Enable_reading();
}

TimerId TimerQueue::Add_timer(std::function<void()> callback, TimeStamp when, double interval) {
    Timer* timer_ = new Timer(callback, when, interval);
    loop->Assert_in_loop_thread();
    loop->Run_in_loop(std::bind(&TimerQueue::Add_timer_in_loop, this, timer_));
    return TimerId(timer_);
}

TimerQueue::~TimerQueue() {
    close(timerfd);
    for (const Entry& timer : timers) {
        delete timer.second;
    }
}

/*

bool TimerQueue::Insert(Timer* timer) {
    loop->Assert_in_loop_thread();
    assert(timers.size() == active_timers.size());
    bool earlist_changed = false;
    TimeStamp when = timer->Expiration();
    TimerList::iterator it = timers.begin();
    if (it == timers.end() || when < it->first) {
        earlist_changed = true;
    }
    std::pair<TimerList::iterator, bool> result1 =
        timers.insert(Entry(when, timer));
    assert(result1.second);
    (void)result1;
    std::pair<ActiveTimerSet::iterator, bool> result2 =
        active_timers.insert(ActiveTimer(timer, timer->Sequence()));
    assert(result2.second);
    (void)result2;
    assert(timers.size() == active_timers.size());
    return earlist_changed;
}

*/
