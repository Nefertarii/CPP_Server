#include "Head/timerqueue.h"
#include "Head/timestamp.h"
#include "Head/timerid.h"
#include "Head/timer.h"
#include "Head/clock.h"
#include "../Net/Poll/Head/eventloop.h"
#include <cassert>
#include <sys/timerfd.h>
#include <string.h>
#include <iostream>

using namespace Wasi::Time;

void TimerQueue::Handle_read() {

}

std::vector<Entry> TimerQueue::Get_expired(TimeStamp now) {
    std::vector<Entry> expired;
    Entry sentry = std::make_pair(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    TimerList::iterator it = timers.lower_bound(sentry);
    assert(it == timers.end() || now < it->first);
    std::copy(timers.begin(), it, back_inserter(expired));
    timers.erase(timers.begin(), it);
    return expired;
}

void TimerQueue::Reset(const std::vector<Entry>& expired, TimeStamp now) {
    TimeStamp next;
}

void TimerQueue::Reset_timerfd(int timerfd_, TimeStamp expiration) {
    itimerspec new_value, old_value;
    memset(&new_value, 0, sizeof(new_value));
    memset(&old_value, 0, sizeof(old_value));
    new_value.it_value = Time_from_now(expiration);
    int ret = timerfd_settime(timerfd, 0, &new_value, &old_value);
    if (ret) { std::cout << "timerfd not set."; }
}

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

int TimerQueue::Create_timerfd() {
    int timerfd_ = ::timerfd_create(CLOCK_MONOTONIC,
                                   TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd_ < 0) { std::cout<<"Fail create timerfd"; }
    return timerfd_;
}

void TimerQueue::Add_timer_in_loop(Timer* timer) {
    //loop->Assert_in_loop_thread()TimerQueue::oseconds_since_epoch() - 
    loop->Assert_in_loop_thread();
    bool earlist_changed = Insert(timer);
    if (earlist_changed) { Reset_timerfd(timerfd, timer->Expiration()); }
}

timespec TimerQueue::Time_from_now(TimeStamp when) {
    long microseconds = when.Microseconds_since_epoch() - Clock::Nowtime_us();
    if (microseconds < 100) { microseconds = 100; }
    timespec ts;
    ts.tv_sec = static_cast<time_t>(microseconds / TimeStamp::microseconds_per_second);
    ts.tv_sec = static_cast<long>((microseconds % TimeStamp::microseconds_per_second) * 1000);
    return ts;
}

TimerQueue::TimerQueue(Net::EventLoop* loop_) :
    timerfd(Create_timerfd()), loop(loop_), timerfd_channel(loop, timerfd), timers() {
    timerfd_channel.Set_read_callback(std::bind(&TimerQueue::Handle_read, this));
    timerfd_channel.Enable_reading();
}

TimerId TimerQueue::Add_timer(const std::function<void()>& callback,
                              TimeStamp when, double interval) {
    Timer* timer = new Timer(callback, when, interval);
    loop->Assert_in_loop_thread();
    bool earliest_changed = Insert(timer);
    if (earliest_changed) { Reset_timerfd(timerfd, timer->Expiration()); }
    return TimerId(timer);
    /*
    loop->Run_in_loop(std::bind(&TimerQueue::Add_timer_in_loop, this, timer));
    return TimerId(timer, timer->Sequence());*/
}

//void Cancel();
TimerQueue::~TimerQueue() {
    timerfd_channel.Disable_reading();
    //timerfd_channel.remove;
    ::close(timerfd);
    for (const Entry& timer : timers) {
        delete timer.second;
    }
}




