#include "Include/Eventloop/poller.h"
#include "Include/Logger/logger.h"
#include "Include/Timer/clock.h"
#include <algorithm>
#include <cstring>

using namespace wasi;
using namespace wasi::loop;

const int Poller::init_events_size_ = 16;

void Poller::FillActiveChannel(int event_nums, ChannelList* active_channels) {
    if (event_nums > (int)events_.size()) {
        LOG_WARN("event_nums greater than events_ size");
    }
    for (int i = 0; i < event_nums; ++i) {
        Channel* channel              = static_cast<Channel*>(events_[i].data.ptr);
        int fd                        = channel->GetFd();
        ChannelMap::const_iterator it = channels_.find(fd);
        if (it == channels_.end()) {
            LOG_WARN("channels_ not find channel fd:" + std::to_string(fd));
        }
        if (it->second != channel) {
            LOG_WARN("active_channels channel not equal channels_ channel. channel fd:" + std::to_string(fd));
        }
        channel->SetRevents(events_[i].events);
        active_channels->push_back(channel);
    }
}

void Poller::Update(int operation, Channel* channel) {
    epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events   = channel->GetEvents();
    event.data.ptr = channel;
    int fd         = channel->GetFd();
    std::string msg;
    if (epoll_ctl(epollfd_, operation, fd, &event) < 0) {
        int tmp_errno = errno;
        LOG_ERROR("epoll:" + std::to_string(epollfd_) + " error"
                  + ", operation:" + OperationToString(operation)
                  + ", errno:" + std::to_string(tmp_errno));
    }
}

std::string Poller::OperationToString(int operation) {
    switch (operation) {
    case EPOLL_CTL_ADD:
        return "Epoll add";
    case EPOLL_CTL_DEL:
        return "Epoll del";
    case EPOLL_CTL_MOD:
        return "Epoll mod";
    default:
        return "Unknown operation";
    }
}

Poller::Poller() :
    events_(init_events_size_),
    epollfd_(epoll_create(EPOLL_CLOEXEC)) {
    if (epollfd_ < 0) {
        LOG_CRIT("epollfd create fail");
    }
}

timer::TimeStamp Poller::Poll(int timeout_ms, ChannelList* active_channels) {
    int num_events = epoll_wait(epollfd_, &*events_.begin(), (int)events_.size(), timeout_ms);
    int errno_     = errno;
    timer::TimeStamp now(timer::Clock::NowtimeMs());
    if (num_events > 0) {
        FillActiveChannel(num_events, active_channels);
        if ((size_t)num_events == events_.size()) { events_.resize(events_.size() * 2); }
    } else if (num_events == 0) {
    } else {
        if (errno_ != EINTR) {
            errno = errno_;
            LOG_ERROR("Poll error");
        }
    }
    return now;
}

void Poller::UpdateChannel(Channel* channel) {
    const int index = channel->GetIndex();
    if (index == kNew || index == kDeleted) {
        // a new one, add with EPOLL_CTL_ADD
        int fd = channel->GetFd();
        if (index == kNew) {
            channels_[fd] = channel;
        } else {
        } // index == kDeleted
        channel->SetIndex(kAdded);
        Update(EPOLL_CTL_ADD, channel);
    } else {
        // LOG_WARN("Can't find channel fd:" + std::to_string(fd));
        // update existing one with EPOLL_CTL_MOD/DEL
        int fd = channel->GetFd();
        // (void)fd;
        if (channels_.find(fd) == channels_.end()) {
            LOG_WARN("Can't find channel fd:" + std::to_string(fd));
        }
        if (channels_[fd] != channel) {
            LOG_WARN("channels_ not equal channel. channel fd:" + std::to_string(fd));
        }
        if (channel->NoneEvent()) {
            Update(EPOLL_CTL_DEL, channel);
            channel->SetIndex(kDeleted);
        } else {
            Update(EPOLL_CTL_MOD, channel);
        }
    }
}

void Poller::RemoveChannel(Channel* channel) {
    int fd = channel->GetFd();
    if (channels_.find(fd) != channels_.end()) {
        if (channels_[fd] == channel) {
            if (channel->NoneEvent()) {
                int index = channel->GetIndex();
                size_t n  = channels_.erase(fd);
                (void)n;
                if (index == kAdded) {
                    Update(EPOLL_CTL_DEL, channel);
                }
                channel->SetIndex(kNew);
            }
        }
    }
}

Poller::~Poller() { close(epollfd_); }
