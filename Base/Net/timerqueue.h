#ifndef TIMER_TIMERQUEUE_H_
#define TIMER_TIMERQUEUE_H_

#include "channel.h"
#include <Base/Timer/timestamp.h>
#include <Base/noncopyable.h>
#include <functional>
#include <memory>
#include <set>

namespace Wasi {
namespace Poll {

class EventLoop;

} // namespace Poll
namespace Time {

class Timer;

class TimerId;

using Entry          = std::pair<TimeStamp, Timer*>;
using TimerList      = std::set<Entry>;
using ActiveTimer    = std::pair<Timer*, int>;
using ActiveTimerSet = std::set<ActiveTimer>;

class TimerQueue : Noncopyable {
private:
    void Handle_read();
    std::vector<Entry> Get_expired(TimeStamp now);
    void Reset(const std::vector<Entry>& expired, TimeStamp now);
    bool Insert(Timer* timer);
    void Add_timer_in_loop(Timer* timer);
    Poll::EventLoop* loop;
    const int timerfd;
    Poll::Channel timerfd_channel;
    TimerList timers;
    ActiveTimerSet active_timers;
    ActiveTimerSet cancel_timers;
    // void Reset_timerfd(int timerfd_, TimeStamp expiration);
public:
    TimerQueue(Poll::EventLoop* loop_);
    TimerId Add_timer(std::function<void()> callback,
                      TimeStamp when, double interval);
    ~TimerQueue();
    // void Cancel();
};

} // namespace Time
} // namespace Wasi

#endif // !TIMER_TIMERQUEUE_H_