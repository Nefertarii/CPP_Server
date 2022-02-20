#ifndef NET_EVENTLOOP_H_
#define NET_EVENTLOOP_H_

#include <vector>
#include <thread>
#include <unistd.h>
#include <functional>
#include <memory>
#include <mutex>
#include "../../../Class/noncopyable.h"
#include "../../../Timer/Head/timestamp.h"

namespace Wasi {
    namespace Time {
        class TimerId;
        class TimerQueue;
    }
    namespace Net {
        class Channel;
        class Poller;
        using ChannelList = std::vector<Channel*>;
        using Functors = std::function<void()>;
        
        class EventLoop : Noncopyable {
        private:
            bool looping;
            bool quit;
            const pid_t thread_id;
            Time::TimeStamp poll_return_time;
            Channel* current_active_channel;
            std::unique_ptr<Poller> poller;
            std::unique_ptr<Time::TimerQueue> timer_queue;
            ChannelList active_channels;
            std::mutex mtx;
            std::vector<Functors> pending_functors;
            void Abort_not_in_loop_thread();
            void Do_pending_functors();
            bool calling_pending_functors;
            int wake_up_fd;
            /*bool looping;
            bool quit;
            bool calling_pending_function;
            int wakeup_fd;
            std::mutex mtx;
            const pid_t thread_id;
            //Poller* poller;
            std::unique_ptr<Channel> wakeup_channel;
            ChannelList active_channels;
            void Handle_read();
            */
        public:
            EventLoop();
            void Loop();
            void Quit();
            Time::TimeStamp Poll_return_time() const;
            Time::TimerId Run_at(const Time::TimeStamp& time, Functors callback);
            Time::TimerId Run_after(double delay, Functors callback);
            Time::TimerId Run_every(double interval, Functors callback);
            void Update_channel(Channel* channel);
            void Assert_in_loop_thread();
            bool Is_in_loop_thread() const;
            void Queue_in_loop(Functors callback);
            void Run_in_loop(Functors callback);
            void Wake_up();
            ~EventLoop();
            /*
            void Wakeup();
            //
            void Run_in_loop(const std::function<void()>& callback);
            
            void Remove_channel(Channel* channel);
            int Create_event();
            bool Has_channel(Channel* channel);
            ~EventLoop();
            */
        };
    }
}






#endif