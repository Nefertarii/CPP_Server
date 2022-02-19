#ifndef TIME_TIMERQUEUE_H_
#define TIME_TIMERQUEUE_H_

#include <functional>
#include <memory>
#include <set>
#include "../../Net/Poll/Head/channel.h"

namespace Wasi {
    namespace Net {
        class EventLoop;
    }
    namespace Time {
        class Timer;
        class TimerId;
        class TimeStamp;
        using Entry = std::pair<TimeStamp, Timer*>;
        using TimerList = std::set<Entry>;
        using ActiveTimer = std::pair<Timer*, int>;
        using ActiveTimerSet = std::set<ActiveTimer>;

        class TimerQueue {
        private:
            void Handle_read();
            std::vector<Entry> Get_expired(TimeStamp now);
            void Reset(const std::vector<Entry>& expired, TimeStamp now);
            void Reset_timerfd(int timerfd_, TimeStamp expiration);
            bool Insert(Timer* timer);
            int Create_timerfd();
            void Add_timer_in_loop(Timer* timer);
            timespec Time_from_now(TimeStamp when);
            const int timerfd;
            Net::EventLoop* loop;
            Net::Channel timerfd_channel;
            TimerList timers;
            ActiveTimerSet active_timers;
            ActiveTimerSet cancel_timers;
        public:
            TimerQueue(Net::EventLoop* loop_);
            TimerId Add_timer(const std::function<void()>& callback,
                              TimeStamp when, double interval);
            //void Cancel();
            ~TimerQueue();
        };
    }
}


#endif