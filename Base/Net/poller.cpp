#include "poller.h"
#include "../Log/logging.h"
#include "Timer/clock.h"
#include "channel.h"
#include "eventloop.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>

using namespace Wasi;
using namespace Wasi::Poll;

const int poller_new = -1;
const int poller_add = 1;
const int poller_del = 2;

const char* Poller::Operation_to_string(int operation) {
    switch (operation) {
    case EPOLL_CTL_ADD:
        return "Epoll add";
    case EPOLL_CTL_DEL:
        return "Epoll del";
    case EPOLL_CTL_MOD:
        return "Epoll mod";
    default:
        assert(false && "ERROR operation");
        return "Unknown operation";
    }
}

void Poller::Fill_active_channel(int num_events, ChannelList* active_channels) const {
    assert((size_t)num_events <= events.size());
    for (int i = 0; i < num_events; ++i) {
        Channel* channel              = static_cast<Channel*>(events[i].data.ptr);
        int fd                        = channel->Fd();
        ChannelMap::const_iterator it = channels.find(fd);
        assert(it != channels.end());
        assert(it->second == channel);
        channel->Set_revents(events[i].events);
        active_channels->push_back(channel);
    }
}

void Poller::Update(int operation, Channel* channel) {
    epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events    = channel->Events();
    event.data.ptr  = channel;
    int fd          = channel->Fd();
    std::string msg = "epoll_ctl op:" + std::string(Operation_to_string(operation))
                      + " fd:" + std::to_string(fd)
                      + " event:" + std::to_string(channel->Fd());
    LOG_DEBUG(msg);
    if (epoll_ctl(epollfd, operation, fd, &event) < 0) {
        if (operation == EPOLL_CTL_DEL) {
            msg = "epoll_ctl op:" + std::string(Operation_to_string(operation))
                  + " fd:" + std::to_string(fd);
            LOG_DEBUG(msg);
        } else {
            msg = "epoll_ctl op:" + std::string(Operation_to_string(operation))
                  + " fd:" + std::to_string(fd);
            LOG_DEBUG(msg);
        }
    }
}

Poller::Poller(EventLoop* loop) :
    ownerloop(loop),
    events(init_events_size),
    epollfd(epoll_create(EPOLL_CLOEXEC)) {
    if (epollfd < 0) { LOG_ERROR("Epollfd init fail."); }
}

Wasi::Time::TimeStamp Poller::Poll(int timeout_ms, ChannelList* active_channels) {
    int num_events = epoll_wait(epollfd, &*events.begin(), (int)events.size(), timeout_ms);
    LOG_DEBUG("num_events total count:" + std::to_string(num_events));
    int errno_ = errno;
    Wasi::Time::TimeStamp now(Wasi::Time::Clock::Nowtime_ms());
    if (num_events > 0) {
        Fill_active_channel(num_events, active_channels);
        LOG_DEBUG(std::to_string(num_events) + " events happended.");
        if ((size_t)num_events == events.size()) { events.resize(events.size() * 2); }
    } else if (num_events == 0) {
        LOG_DEBUG("Nothing happend.");
    } else {
        if (errno_ != EINTR) {
            errno = errno_;
            LOG_ERROR("Poll error");
        }
    }
    return now;
}
// poller只负责IO multiplexing(多路复用) 不负责dispatching(事件分发)

void Poller::Update_channel(Channel* channel) {
    Assert_in_loop_thread();
    const int index = channel->Index();

    std::string msg = "fd = " + std::to_string(channel->Fd())
                      + " events = " + std::to_string(channel->Events())
                      + " index = " + std::to_string(index);
    LOG_DEBUG(msg);
    if (index == NEW || index == DELETED) {
        // a new one, add with EPOLL_CTL_ADD
        int fd = channel->Fd();
        if (index == NEW) {
            assert(channels.find(fd) == channels.end());
            channels[fd] = channel;
        } else { // index == kDeleted
            assert(channels.find(fd) != channels.end());
            assert(channels[fd] == channel);
        }
        channel->Set_index(ADDED);
        Update(EPOLL_CTL_ADD, channel);
    } else {
        // update existing one with EPOLL_CTL_MOD/DEL
        int fd = channel->Fd();
        (void)fd;
        assert(channels.find(fd) != channels.end());
        assert(channels[fd] == channel);
        assert(index == ADDED);
        if (channel->Is_none_event()) {
            Update(EPOLL_CTL_DEL, channel);
            channel->Set_index(DELETED);
        } else {
            Update(EPOLL_CTL_MOD, channel);
        }
    }
}

void Poller::Remove_channel(Channel* channel) {
    Assert_in_loop_thread();
    int fd_ = channel->Fd();
    LOG_INFO("Remove fd:" + std::to_string(fd_));
    assert(channels.find(fd_) != channels.end());
    assert(channels[fd_] == channel);
    assert(channel->Is_none_event());
    int index = channel->Index();
    assert(index == poller_add || index == poller_del);
    size_t n = channels.erase(fd_);
    (void)n;
    assert(n == 1);
    if (index == poller_add) {
        Update(EPOLL_CTL_DEL, channel);
    }
    channel->Set_index(poller_new);
}

void Poller::Assert_in_loop_thread() const { ownerloop->Is_in_loop_thread(); }

Poller* Poller::New_default_poller(EventLoop* loop) { return new Poller(loop); }

Poller::~Poller() {
    close(epollfd);
}
