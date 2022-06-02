#ifndef POLL_POLLER_H_
#define POLL_POLLER_H_

#include <Base/noncopyable.h>
#include <map>
#include <sys/epoll.h>
#include <vector>

namespace Wasi {
namespace Time {
class TimeStamp;
} // namespace Time

namespace Poll {
class EventLoop;
class Channel;

// using PollFdList = std::vector<pollfd>;
using ChannelList = std::vector<Channel*>;
using ChannelMap  = std::map<int, Channel*>;
using EventList   = std::vector<epoll_event>;
class Poller : Noncopyable {
private:
    enum PollState {
        NEW     = -1,
        ADDED   = 1,
        DELETED = 2
    };
    const char* Operation_to_string(int operation);
    void Fill_active_channel(int num_events, ChannelList* active_channels) const;
    void Update(int operation, Channel* channel);
    static const int init_events_size = 16;
    EventLoop* ownerloop;
    ChannelMap channels;
    EventList events;
    int epollfd;

public:
    Poller(EventLoop* loop);
    Time::TimeStamp Poll(int timeout_ms, ChannelList* active_channels);
    void Update_channel(Channel* channel);
    void Remove_channel(Channel* channel);
    // bool Has_channel(Channel* channel) const;
    void Assert_in_loop_thread() const;
    static Poller* New_default_poller(EventLoop* loop);
    ~Poller();
};

} // namespace Poll
} // namespace Wasi

#endif // !POLL_POLLER_H_