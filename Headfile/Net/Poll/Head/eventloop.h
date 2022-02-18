#ifndef NET_EVENTLOOP_H_
#define NET_EVENTLOOP_H_

#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <functional>
#include "../../../Class/noncopyable.h"

namespace Wasi {
    namespace Time {
        class TimerId;
        class TimeStamp;
        class TimerQueue;
    }
    namespace Net {
        class Channel;
        class Poller;
        using ChannelList = std::vector<Channel*>;
        
        class EventLoop : Noncopyable {
        private:
            bool looping;
            bool quit;
            bool calling_pending_function;
            int wakeup_fd;
            std::mutex mtx;
            std::vector<std::function<void()>> pending_functions;
            const pid_t thread_id;
            std::unique_ptr<Channel> wakeup_channel;
            std::unique_ptr<Time::TimerQueue> timer_queue;
            std::unique_ptr<Poller> poller;
            ChannelList active_channels;
            void Handle_read();
            void Do_pending_functions();
        public:
            EventLoop();
            void Loop();
            void Quit();
            void Wakeup();
            bool Is_in_loop_thread();
            //void Abort_not_in_loop_thread();
            Time::TimerId Run_at(const Time::TimeStamp& time, const std::function<void()>& callback);
            Time::TimerId Run_after(double delay, const std::function<void()>& callback);
            Time::TimerId Run_every(double interval, const std::function<void()>& callback);
            void Run_in_loop(const std::function<void()> callback);
            void Queue_in_loop(const std::function<void()> callback);
            void Assert_in_loop_thread();
            void Update_channel(Channel* channel);
            void Remove_channel(Channel* channel);
            bool Has_channel(Channel* channel);
            ~EventLoop();
        };
    }
}






#endif