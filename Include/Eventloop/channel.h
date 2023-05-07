#ifndef EVENTLOOP_CHANNEL_H_
#define EVENTLOOP_CHANNEL_H_

#include "Include/Timer/timestamp.h"
#include <functional>
#include <memory>

namespace wasi {
namespace loop {
class EventLoop;

using EventCallback     = std::function<void()>;
using ReadEventCallback = std::function<void(timer::TimeStamp)>;

enum ChannelEvent {
    NONEVENT  = 0,
    READEVENT = 3, // POLLIN | POLLPRI
    WRITEVENT = 4  // POLLOUT
};

class Channel {
private:
    // void Update();
    const int fd_;
    bool looping_;
    bool handling_;
    int events_;
    int revents_;
    int index_;
    EventLoop* eventloop_;
    ReadEventCallback read_callback_;
    EventCallback write_callback_;
    EventCallback error_callback_;
    EventCallback close_callback_;
    void Update();

public:
    Channel(EventLoop* loop, int fd);
    void HandleEvent(timer::TimeStamp receive_time);
    void SetRevents(int revents);
    void SetIndex(int index);
    void SetReadCallback(ReadEventCallback callback);
    void SetWriteCallback(EventCallback callback);
    void SetErrorCallback(EventCallback callback);
    void SetCloseCallback(EventCallback callback);
    void EnableReading();
    void EnableWriting();
    void DisableReading();
    void DisableWriting();
    void DisableAll();
    void Remove();
    bool NoneEvent();
    bool Writing();
    bool Reading();
    int GetFd();
    int GetIndex();
    int GetEvents();
    ~Channel();
};

}} // namespace wasi::loop

#endif