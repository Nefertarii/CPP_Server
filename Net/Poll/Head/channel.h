#ifndef POLL_CHANNEL_H_
#define POLL_CHANNEL_H_

#include "../../../Class/noncopyable.h"
#include <functional>
#include <memory>

namespace Wasi {
namespace Time {

class TimeStamp;

} // namespace Time
namespace Poll {

class EventLoop;

class Channel : Noncopyable {
private:
    using EventCallBack     = std::function<void()>;
    using ReadEventCallBack = std::function<void(Time::TimeStamp)>;
    void Update();
    EventLoop* loop;
    const int fd;
    bool in_loop;
    bool event_handling;
    int events;
    int revents;
    int index;
    ReadEventCallBack read_callback;
    EventCallBack write_callback;
    EventCallBack error_callback;
    EventCallBack close_callback;

public:
    enum ChannelEvent {
        NONEVENT  = 0,
        READEVENT = 3, // POLLIN | POLLPRI
        WRITEVENT = 4  // POLLOUT
    };
    Channel(EventLoop* loop_, int fd_);
    int Fd();
    int Index();
    int Events();
    int Revents();
    // void Remove():
    void Handle_event(Time::TimeStamp receive_time);
    // void Handle_event_with_guard(Time::TimeStamp receive_time);
    void Set_revents(int revents_);
    void Set_index(int index_);
    void Set_read_callback(ReadEventCallBack cb);
    void Set_write_callback(EventCallBack cb);
    void Set_error_callback(EventCallBack cb);
    void Set_close_callback(EventCallBack cb);
    void Enable_reading();
    void Disable_reading();
    void Enable_writing();
    void Disable_writing();
    void Disable_all();
    void Remove();
    bool Is_none_event();
    bool Is_writing();
    bool Is_reading();
    EventLoop* Owner_loop();
    ~Channel();
};

} // namespace Poll
} // namespace Wasi

#endif // !NET_CHANNEL_H_
