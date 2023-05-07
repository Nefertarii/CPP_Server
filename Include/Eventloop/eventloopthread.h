#ifndef EVENTLOOP_EVENTLOOPTHREAD_H_
#define EVENTLOOP_EVENTLOOPTHREAD_H_

#include "Include/Header/thread.h"
#include <mutex>

namespace wasi {
namespace loop {
class EventLoop;

using ThreadInitCallback = std::function<void(EventLoop*)>;

class EventLoopThread {
private:
    void ThreadFunc();
    EventLoop* loop_;
    bool exiting_;
    head::Thread thread_;
    std::mutex mtx_;
    std::condition_variable cond_;
    ThreadInitCallback thread_init_callback_;

public:
    EventLoopThread(const ThreadInitCallback& callback = ThreadInitCallback(),
                    const std::string& name            = std::string());
    EventLoop* StartLoop();
    ~EventLoopThread();
};

}
} // namespace wasi::loop

#endif