#include "Head/eventloopthread.h"
#include "Head/eventloop.h"
#include <cassert>

using namespace Wasi::Net;

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

EventLoopThread::EventLoopThread() :
    loop(nullptr),
    exiting(false),
    thread(std::bind(&EventLoopThread::Thread_func, this)),
    mtx(),
    cond_mtx(),
    cond() {}

EventLoop* EventLoopThread::Start_loop() {
    assert(!thread.Started());
    thread.Start();
    {
        std::lock_guard<std::mutex> lk(mtx);
        while (loop == nullptr) {
            std::unique_lock<std::mutex> cond_lk(cond_mtx);
            cond.wait(cond_lk);
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