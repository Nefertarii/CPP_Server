#include "Head/eventloopthreadpool.h"
#include "Head/eventloop.h"
#include <cassert>

using namespace Wasi::Poll;
using namespace Wasi;

// EventLoop* baseloop;
// Base::ThreadPool thread_pool;
// std::string name;
// bool started;
// int num_threads;
// std::vector<std::unique_ptr<EventLoopThread>> threads;
// std::vector<EventLoop*> loops;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseloop_, const std::string& name_) :
    baseloop(baseloop_),
    thread_pool(),
    loops(),
    name(name_),
    started(false),
    num_threads(0),
    index(0) {
    // thread_pool;
}

void EventLoopThreadPool::Set_thread_num(int num_threads_) { num_threads = num_threads_; }

void EventLoopThreadPool::Start(const ThreadInitCallback& cb) {
    assert(!started);
    baseloop->Assert_in_loop_thread();
    started = true;
    for (int i = 0; i < num_threads; ++i) {
        loops.push_back(std::make_unique<EventLoop>());
    }
    if (num_threads == 0 && cb) {
        baseloop->Run_in_loop(cb);
    }
}

EventLoop* EventLoopThreadPool::Get_loop() {
    baseloop->Assert_in_loop_thread();
    assert(started);
    EventLoop* loop = baseloop;
    if (!loops.empty()) {
        loop = loops[index];
        ++index;
        if (static_cast<size_t>(index) >= loops.size()) {
            index = 0;
        }
    }
    return loop;
}

std::string EventLoopThreadPool::Get_name() const { return name; }

bool EventLoopThreadPool::Started() { return started; }

EventLoopThreadPool::~EventLoopThreadPool() {
    thread_pool.Stop();
}