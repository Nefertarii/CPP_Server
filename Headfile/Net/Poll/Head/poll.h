#ifndef NET_POLL_H_
#define NET_POLL_H_

#include "../../../Class/noncopyable.h"
#include "../../../Timer/Head/timer.h"
#include <vector>
#include <map>

struct pollfd;

namespace Wasi {
    namespace Net {
        class Channel;
        class EventLoop;
        using PollFdList = std::vector<struct pollfd>;
        using ChannelList = std::vector<Channel*>;
        using ChannelMap = std::map<int, Channel*>;

        class Poller : Noncopyable {
        private:
            void Fill_active_channel(int events, ChannelList* active_channels) const;
            EventLoop* ownerloop;
            PollFdList pollfds;
            ChannelMap channels;
        public:
            Poller(EventLoop* loop);
            Wasi::Time::TimeStamp Poll(int timeout_ms, ChannelList* active_channels);
            void Update_channel(Channel* channel);
            bool Has_channel(Channel* channel) const;
            void Assert_in_loop_thread() const;
            ~Poller();
        };
    }
}




#endif