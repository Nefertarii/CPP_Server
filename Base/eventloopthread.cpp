#include "eventloopthread.h"
#include "eventloop.h"
#include <cassert>

using namespace Wasi::Poll;

void EventLoopThread::Thread_func() {
    EventLoop tmp_loop;
    {
        std::lock_guard<std::mutex> lk(mtx);
        loop = &tmp_loop;
        cond.notify_all();
    }
    tmp_loop.Loop();
    loop = nullptr;
}

EventLoopThread::EventLoopThread(const ThreadInitCallback& callback_, const std::string& name) :
    loop(nullptr),
    exiting(false),
    thread(std::bind(&EventLoopThread::Thread_func, this), name),
    cond(),
    callback(callback_) {}

EventLoop* EventLoopThread::Start_loop() {
    assert(!thread.Started());
    thread.Start();
    {
        std::unique_lock<std::mutex> lk(mtx);
        while (loop == nullptr) {
            cond.wait(lk);
        }
    }
    return loop;
}

EventLoopThread::~EventLoopThread() {
    exiting = true;
    if (loop != nullptr) {
        loop->Quit();
        thread.Join();
    }
}