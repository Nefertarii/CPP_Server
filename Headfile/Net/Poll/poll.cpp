#include "Head/poll.h"
#include "Head/eventloop.h"
#include "Head/channel.h"
#include <sys/poll.h>
#include <algorithm>

using namespace Wasi::Net;

Poller::Poller(EventLoop* loop) :ownerloop(loop) {}

Wasi::Time::TimeStamp Poller::Poll(int timeout_ms, ChannelList* active_channels) {
    int num_events = ::poll(&*pollfds.begin(), pollfds.size(), timeout_ms);
    Wasi::Time::TimeStamp now(Wasi::Time::Timer::Nowtime_us());
    if (num_events > 0) {
        Fill_active_channel(num_events, active_channels);
        //LOG events happended
    }
    else if (num_events == 0) {
        //LOG nothing happended
    }
    else {
        //LOG err 
    }
    return now;
}
//poller只负责IO multiplexing(多路复用) 不负责dispatching(事件分发)

void Poller::Fill_active_channel(int events, ChannelList* active_channels) const {
    for (PollFdList::const_iterator pfd = pollfds.begin();
         pfd != pollfds.end() && events > 0; ++pfd) {
        if (pfd->revents > 0) {
            --events;
            ChannelMap::const_iterator ch = channels.find(pfd->fd);
            assert(ch != channels.end());
            Channel* channel = ch->second;
            assert(channel->Fd() == pfd->fd);
            channel->Set_revents(pfd->revents);
            active_channels->push_back(channel);
        }
    }
}

void Poller::Update_channel(Channel* channel) {
    Assert_in_loop_thread();
    //LOG fd = channel->Fd() events = channel->Events();
    if (channel->Index() < 0) {
        assert(channels.find(channel->Fd()) == channels.end());
        struct pollfd pfd;
        pfd.fd = channel->Fd();
        pfd.events = static_cast<short>(channel->Events());
        pfd.revents = 0;
        pollfds.push_back(pfd);
        int idx = static_cast<int>(pollfds.size()) - 1;
        channel->Set_index(idx);
        channels[pfd.fd] = channel;
    }
    else {
        assert(channels.find(channel->Fd()) != channels.end());
        assert(channels[channel->Fd()] == channel);
        int idx = channel->Index();
        assert(0 <= idx && idx < static_cast<int>(pollfds.size()));
        struct pollfd& pfd = pollfds[idx];
        assert(pfd.fd == channel->Fd() || pfd.fd == -1);
        pfd.revents = 0;
        if (channel->Is_none_event()) {
            pfd.fd = -1;
        }
    }
}

bool Poller::Has_channel(Channel* channel) const {
    this->Assert_in_loop_thread();
    ChannelMap::const_iterator it = channels.find(channel->Fd());
    return it != channels.end() && it->second == channel;
}

void Poller::Assert_in_loop_thread() const { ownerloop->Is_in_loop_thread(); }

Poller::~Poller() {

}
