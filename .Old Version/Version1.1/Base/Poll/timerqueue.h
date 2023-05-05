#ifndef TIMER_TIMERQUEUE_H_
#define TIMER_TIMERQUEUE_H_

#include <Base/Poll/channel.h>
#include <Base/Timer/timestamp.h>
#include <Base/noncopyable.h>
#include <functional>
#include <memory>
#include <set>

namespace Wasi {
namespace Time {
class Timer;
class TimerId;
} // namespace Time

namespace Poll {
class EventLoop;

using Entry          = std::pair<Time::TimeStamp, Time::Timer*>;
using TimerList      = std::set<Entry>;
using ActiveTimer    = std::pair<Time::Timer*, int>;
using ActiveTimerSet = std::set<ActiveTimer>;

class TimerQueue : Noncopyable {
private:
    void Handle_read();
    std::vector<Entry> Get_expired(Time::TimeStamp now);
    void Reset(const std::vector<Entry>& expired, Time::TimeStamp now);
    bool Insert(Time::Timer* timer);
    void Add_timer_in_loop(Time::Timer* timer);
    Poll::EventLoop* loop;
    const int timerfd;
    Channel timerfd_channel;
    TimerList timers;
    ActiveTimerSet active_timers;
    ActiveTimerSet cancel_timers;
    // void Reset_timerfd(int timerfd_, TimeStamp expiration);
public:
    TimerQueue(Poll::EventLoop* loop_);
    Time::TimerId Add_timer(std::function<void()> callback,
                            Time::TimeStamp when, double interval);
    ~TimerQueue();
    // void Cancel();
};

} // namespace Poll
} // namespace Wasi

#endif // !TIMER_TIMERQUEUE_H_