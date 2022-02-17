#ifndef NET_EVENTLOOP_H_
#define NET_EVENTLOOP_H_

#include <vector>
#include <thread>
#include <unistd.h>
#include <assert.h>
#include "../../../Class/noncopyable.h"

namespace Wasi {
    namespace Net {
        class Channel;
        class Poller;
        using ChannelList = std::vector<Channel*>;
        class EventLoop : Noncopyable {
        private:
            bool looping;
            bool quit;
            const pid_t thread_id;
            std::unique_ptr<Poller> poller;
            ChannelList active_channels;
        public:
            EventLoop();
            void Loop();
            void Quit();
            bool Is_in_loop_thread();
            //void Abort_not_in_loop_thread();
            void Assert_in_loop_thread();
            void Update_channel(Channel* channel);
            void Remove_channel(Channel* channel);
            bool Has_channel(Channel* channel);
            ~EventLoop();
        };
    }
}






#endif