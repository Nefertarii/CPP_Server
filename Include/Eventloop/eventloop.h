#ifndef EVENTLOOP_EVENTLOOP_H_
#define EVENTLOOP_EVENTLOOP_H_

#include "Include/Eventloop/poller.h"
#include <mutex>
#include <thread>
#include <unistd.h>

namespace wasi {
namespace loop {
class TimerQueue;
class Channel;
class Poller;

using Function = std::function<void()>;
using ChannelList = std::vector<Channel*>;

class EventLoop {
private:
    int wakeup_fd_;
    const int poll_timeout_ms;
    bool looping_;
    bool event_handling_;
    bool calling_pending_;
    bool quit_;
    const pid_t thread_id_;
    std::mutex mtx_;
    Channel* active_channel_;
    ChannelList channels_;
    timer::TimeStamp poller_return_time;
    std::vector<Function> pending_functions;
    std::unique_ptr<Poller> poller_;
    std::unique_ptr<Channel> wakeup_channel_;
    std::unique_ptr<TimerQueue> timer_queue_;
    int CreateEventFd();
    void RunPendingFunction();
    void HandleRead();

public:
    EventLoop();
    void Loop();
    void Quit();
    timer::TimeStamp GetReturnTime() const;
    void RunAt(const timer::TimeStamp& time, Function func);
    void RunAfter(double delay, Function func);
    void RunEvery(double interval, Function func);
    void UpdateChannel(Channel* channel);
    void RemoveChannel(Channel* channel);
    bool IsInLoopThread() const;
    void QueueInLoop(Function func);
    void RunInLoop(Function func);
    void Wakeup();
    ~EventLoop();
};

}} // namespace wasi::loop

#endif