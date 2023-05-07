#ifndef EVENTLOOP_TIMEQUEUE_H_
#define EVENTLOOP_TIMEQUEUE_H

#include "Include/Timer/timestamp.h"
#include "Include/Timer/timer.h"
#include "Include/Eventloop/channel.h"
#include <vector>
#include <functional>
#include <memory>
#include <set>

namespace wasi {
namespace loop {
class EventLoop;

using Expired        = std::pair<timer::TimeStamp, timer::Timer*>;
using TimerList      = std::set<Expired>;
using ActiveTimer    = std::pair<timer::Timer*, int>;
using ActiveTimerSet = std::set<ActiveTimer>;

class TimerQueue {
private:
    int CreateTimerfd();
    void ReadTimerfd(int timerfd, timer::TimeStamp now);
    void ResetTimerfd(int timerfd, timer::TimeStamp expiration);
    timespec TimeFromNow(timer::TimeStamp when);
    void HandleRead();
    std::vector<Expired> GetExpired(timer::TimeStamp now);
    bool Insert(timer::Timer* timer);
    void AddTimerInLoop(timer::Timer* timer);
    void Reset(const std::vector<Expired>& expired, timer::TimeStamp now);
    const int timerfd_;
    EventLoop* eventloop_;
    Channel timerfd_channel_;
    TimerList timers_;
    ActiveTimerSet active_timers_;
    ActiveTimerSet cancel_timers_;

public:
    TimerQueue(EventLoop* eventloop);
    void AddTimer(std::function<void()> callback, timer::TimeStamp when, double interval);
    ~TimerQueue();
};

}} // namespace wasi::loop

#endif
