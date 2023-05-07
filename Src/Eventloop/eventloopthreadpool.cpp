#include "Include/Eventloop/eventloop.h"
#include "Include/Eventloop/eventloopthread.h"
#include "Include/Eventloop/eventloopthreadpool.h"

using namespace wasi;
using namespace wasi::loop;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop, const std::string& name) :
    started_(false),
    index_(0),
    num_threads_(0),
    name_(name),
    baseloop_(loop),
    threads_(),
    loops_() {}

void EventLoopThreadPool::SetThreadNum(int nums) { num_threads_ = nums; }

void EventLoopThreadPool::Start(const ThreadInitCallback& cb) {
    if (!started_) {
        if (baseloop_->IsInLoopThread()) {
            started_ = true;
            for (int i = 0; i < num_threads_; ++i) {
                threads_.push_back(std::make_unique<EventLoopThread>(cb, (name_ + std::to_string(i))));
                loops_.push_back(threads_[i]->StartLoop());
            }
            if (num_threads_ == 0 && cb) {
                cb(baseloop_);
            }
        }
    }
}

EventLoop* EventLoopThreadPool::GetLoop() {
    if (baseloop_->IsInLoopThread()) {
        if (started_) {
            EventLoop* loop = baseloop_;
            if (!loops_.empty()) {
                loop = loops_[index_];
                ++index_;
                if (static_cast<size_t>(index_) >= loops_.size()) {
                    index_ = 0;
                }
            }
            return loop;
        }
    }
    return nullptr;
}

std::string EventLoopThreadPool::GetName() const { return name_; }

int EventLoopThreadPool::GetThreadNum() const { return num_threads_; }

bool EventLoopThreadPool::Started() { return started_; }

EventLoopThreadPool::~EventLoopThreadPool() { /*stack variable*/ }