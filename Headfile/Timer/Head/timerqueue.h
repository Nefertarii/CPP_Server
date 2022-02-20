#ifndef TIME_TIMERQUEUE_H_
#define TIME_TIMERQUEUE_H_

#include <functional>
#include <memory>
#include <set>
#include "../../Net/Poll/Head/channel.h"
#include "../../Class/noncopyable.h"
#include "timestamp.h"

namespace Wasi {
    namespace Net {
        class EventLoop;
    }
    namespace Time {
        class Timer;
        class TimerId;
        using Entry = std::pair<TimeStamp, Timer*>;
        using TimerList = std::set<Entry>;
        using ActiveTimer = std::pair<Timer*, int>;
        using ActiveTimerSet = std::set<ActiveTimer>;

        class TimerQueue : Noncopyable{
        private:
            void Handle_read();
            std::vector<Entry> Get_expired(TimeStamp now);
            void Reset(const std::vector<Entry>& expired, TimeStamp now);
            bool Insert(Timer* timer);
            void Add_timer_in_loop(Timer* timer);
            Net::EventLoop* loop;
            const int timerfd;
            Net::Channel timerfd_channel;
            TimerList timers;
            ActiveTimerSet active_timers;
            ActiveTimerSet cancel_timers;
            //void Reset_timerfd(int timerfd_, TimeStamp expiration);
        public:
            TimerQueue(Net::EventLoop* loop_);
            TimerId Add_timer(std::function<void()> callback,
                              TimeStamp when, double interval);
            ~TimerQueue();
            //void Cancel();
        };
    }
}


#endif