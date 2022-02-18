#include "Head/timerqueue.h"
#include "Head/timestamp.h"
#include "Head/timerid.h"
#include "Head/timer.h"
#include "../Net/Poll/Head/eventloop.h"
#include <cassert>
#include <sys/timerfd.h>

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
    ;
}

bool TimerQueue::Insert(Timer* timer) {
    return true;
}

int TimerQueue::Create_timerfd() {
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                   TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0) {
        //std::cout<<"Fail create timerfd";
    }
    return timerfd;
}

TimerQueue::TimerQueue(Net::EventLoop* loop_) :
    timerfd(Create_timerfd()), loop(loop_), timerfd_channel(loop, timerfd),
    timers() {
    timerfd_channel.Set_read_callback(std::bind(&TimerQueue::Handle_read, this));
    timerfd_channel.Enable_reading();
}

TimerId TimerQueue::Add_timer(const std::function<void()> callback,
                              TimeStamp when, double interval) {
    Timer* timer = new Timer(std::move(callback), when, interval);
    loop->Run_in_loop(std::move(callback));
    return TimerId(timer, timer->Sequence());
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




