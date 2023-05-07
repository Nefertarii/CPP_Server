#ifndef EVENTLOOP_POLLER_H_
#define EVENTLOOP_POLLER_H_

#include "Include/Timer/timestamp.h"
#include "Include/Eventloop/channel.h"
#include <map>
#include <sys/epoll.h>
#include <vector>

namespace wasi {
namespace loop {

using ChannelList = std::vector<Channel*>;
using ChannelMap  = std::map<int, Channel*>;
using EventList   = std::vector<epoll_event>;

enum PollerState {
    kNew     = -1,
    kAdded   = 1,
    kDeleted = 2
};

class Poller {
private:
    static const int init_events_size_;
    ChannelMap channels_;
    EventList events_;
    int epollfd_;
    void FillActiveChannel(int event_nums, ChannelList* active_channels);
    void Update(int operation, Channel* channel);
    std::string OperationToString(int operation);

public:
    Poller();
    timer::TimeStamp Poll(int timeout_ms, ChannelList* active_channels);
    void UpdateChannel(Channel* channel);
    void RemoveChannel(Channel* channel);
    ~Poller();
};

}} // namespace wasi::loop

#endif