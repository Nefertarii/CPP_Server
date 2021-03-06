#ifndef POLL_EVENTLOOP_H_
#define POLL_EVENTLOOP_H_

#include <Base/Timer/timestamp.h>
#include <Base/noncopyable.h>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>

namespace Wasi {
namespace Time {
class TimerId;
} // namespace Time

namespace Poll {
class Poller;
class TimerQueue;
class Channel;

using ChannelList = std::vector<Channel*>;
using Functors    = std::function<void()>;

class EventLoop : Noncopyable {
private:
    int wake_up_fd;
    bool looping;
    bool event_handling;
    bool quit;
    bool calling_pending_functors;
    const pid_t thread_id;
    Time::TimeStamp poll_return_time;
    Channel* current_active_channel;
    std::unique_ptr<Poller> poller;
    std::unique_ptr<TimerQueue> timer_queue;
    ChannelList active_channels;
    std::mutex mtx;
    std::vector<Functors> pending_functors;
    std::unique_ptr<Channel> wakeup_channel;
    void Abort_not_in_loop_thread();
    void Do_pending_functors();
    void Handle_read();

public:
    EventLoop();
    void Loop();
    void Quit();
    Time::TimeStamp Poll_return_time() const;
    Time::TimerId Run_at(const Time::TimeStamp& time, Functors callback);
    Time::TimerId Run_after(double delay, Functors callback);
    Time::TimerId Run_every(double interval, Functors callback);
    void Update_channel(Channel* channel);
    void Remove_channel(Channel* channel);
    void Assert_in_loop_thread();
    bool Is_in_loop_thread() const;
    void Queue_in_loop(Functors callback);
    void Run_in_loop(Functors callback);
    void Wake_up();
    ~EventLoop();
};

} // namespace Poll
} // namespace Wasi

#endif // !POLL_EVENTLOOP_H_