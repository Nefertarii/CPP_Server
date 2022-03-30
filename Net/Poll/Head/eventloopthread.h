#ifndef POLL_EVENTLOOP_THREAD_H_
#define POLL_EVENTLOOP_THREAD_H_

#include "../../../Class/noncopyable.h"
#include "../../Base/Head/thread.h"
#include <condition_variable>
#include <mutex>

namespace Wasi {
namespace Poll {

class EventLoop;

using ThreadInitCallback = std::function<void(EventLoop*)>;

class EventLoopThread : Noncopyable {
private:
    void Thread_func();
    EventLoop* loop;
    bool exiting;
    Base::Thread thread;
    std::mutex mtx;
    std::condition_variable cond;
    ThreadInitCallback callback;

public:
    /*
    EventLoopThread(const ThreadInitCallback& callback_ = ThreadInitCallback(),
                    const std::string& name = std::string());
    */
    EventLoopThread();
    EventLoop* Start_loop();
    ~EventLoopThread();
};

}
} // namespace Wasi::Poll

#endif // !POLL_EVENTLOOP_THREAD_H_