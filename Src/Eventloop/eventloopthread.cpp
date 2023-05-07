#include "Include/Eventloop/eventloop.h"
#include "Include/Eventloop/eventloopthread.h"

using namespace wasi;
using namespace wasi::loop;

void EventLoopThread::ThreadFunc() {
    EventLoop loop;
    {
        std::lock_guard<std::mutex> lk(mtx_);
        loop_ = &loop;
        cond_.notify_all();
    }
    loop.Loop();
    loop_ = nullptr;
}

EventLoopThread::EventLoopThread(const ThreadInitCallback& callback, const std::string& name) :
    loop_(nullptr),
    exiting_(false),
    thread_(std::bind(&EventLoopThread::ThreadFunc, this), name),
    mtx_(),
    cond_(),
    thread_init_callback_(callback) {}

EventLoop* EventLoopThread::StartLoop() {
    if (!thread_.Started()) {
        thread_.Start();
        {
            std::unique_lock<std::mutex> lk(mtx_);
            while (loop_ == nullptr) {
                cond_.wait(lk);
            }
        }
        return loop_;
    }
    return nullptr;
}

EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    if (loop_ != nullptr) {
        loop_->Quit();
        thread_.Join();
    }
}