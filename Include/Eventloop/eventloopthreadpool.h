#ifndef EVENTLOOP_EVENTLOOPTHREADLOOP_H_
#define EVENTLOOP_EVENTLOOPTHREADLOOP_H_

#include "Include/Eventloop/eventloopthread.h"
#include <functional>
#include <memory>
#include <vector>

namespace wasi {
namespace loop {
class EventLoop;
class EventLoopThread;

using ThreadInitCallback = std::function<void(EventLoop*)>;

class EventLoopThreadPool {
private:
    bool started_;
    int index_;
    int num_threads_;
    std::string name_;
    EventLoop* baseloop_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;

public:
    EventLoopThreadPool(EventLoop* loop, const std::string& name);
    void SetThreadNum(int nums);
    void Start(const ThreadInitCallback& cb = ThreadInitCallback());
    EventLoop* GetLoop();
    std::string GetName() const;
    int GetThreadNum() const;
    bool Started();
    ~EventLoopThreadPool();
};

}
} // namespace wasi::loop

#endif