#include "Head/eventloopthreadpool_muduo.h"
#include "Head/eventloop.h"
#include "Head/eventloopthread.h"
#include <cassert>

using namespace Wasi::Poll;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseloop_, const std::string& name_) :
    baseloop(baseloop_),
    name(name_),
    started(false),
    num_threads(0),
    next(0) {}

void EventLoopThreadPool::Set_thread_num(int num_threads_) {
    num_threads = num_threads_;
}

void EventLoopThreadPool::Start(const ThreadInitCallback& cb) {
    assert(!started);
    baseloop->Assert_in_loop_thread();
    started = true;
    for (int i = 0; i < num_threads; ++i) {
        // EventLoopThread* tmp =
        threads.push_back(std::make_unique<EventLoopThread>(cb, (name + std::to_string(i))));
        loops.push_back(threads[i]->Start_loop());
    }
    if (num_threads == 0 && cb) {
        cb(baseloop);
    }
}

EventLoop* EventLoopThreadPool::Get_next_loop() {
    baseloop->Assert_in_loop_thread();
    assert(started);
    EventLoop* loop = baseloop;
    if (!loops.empty()) {
        loop = loops[next];
        ++next;
        if (static_cast<size_t>(next) >= loops.size()) {
            next = 0;
        }
    }
    return loop;
}

EventLoop* EventLoopThreadPool::Get_loop_hash(size_t hash) {
    baseloop->Assert_in_loop_thread();
    EventLoop* loop = baseloop();
    if (!loops.empty()) {
        loop = loops[hash % loops.size()];
    }
    return loop;
}

std::string EventLoopThreadPool::Get_name() const { return name; }

std::vector<EventLoop*> EventLoopThreadPool::Get_all_loops() {
    baseloop->Assert_in_loop_thread();
    assert(started);
    if (loops.empty()) {
        return std::vector<EventLoop*>(1, baseloop);
    } else {
        return loops;
    }
}

bool EventLoopThreadPool::Started() { return started; }

EventLoopThreadPool::~EventLoopThreadPool();