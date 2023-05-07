#include "Include/Eventloop/eventloop.h"
#include "Include/Eventloop/channel.h"
#include "Include/Eventloop/timequeue.h"
#include "Include/Eventloop/poller.h"
#include "Include/Timer/clock.h"
#include "Include/Logger/logger.h"
#include <sys/eventfd.h>
#include <signal.h>
#include <sstream>

using namespace wasi;
using namespace wasi::loop;

int EventLoop::CreateEventFd() {
    int event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (event_fd < 0) {
        LOG_CRIT("Event fd create fail.");
    }
    return event_fd;
}

void EventLoop::RunPendingFunction() {
    std::vector<Function> functions;
    calling_pending_ = true;
    {
        std::lock_guard<std::mutex> lk(mtx_);
        functions.swap(pending_functions);
    }
    for (const Function& function : functions) { function(); }
    calling_pending_ = false;
}

void EventLoop::HandleRead() {
    ssize_t tmp = 1;
    ssize_t ret = ::read(wakeup_fd_, &tmp, sizeof(tmp));
    if (ret != sizeof(tmp)) {
        LOG_ERROR("read to " + std::to_string(wakeup_fd_) + " " + std::to_string(ret) + " bytes, should be 8");
    }
}

EventLoop::EventLoop() :
    wakeup_fd_(CreateEventFd()),
    poll_timeout_ms(5000),
    looping_(false),
    event_handling_(false),
    calling_pending_(false),
    quit_(false),
    thread_id_(gettid()),
    mtx_(),
    active_channel_(nullptr),
    channels_(),
    poller_return_time(),
    pending_functions(),
    poller_(std::make_unique<Poller>()),
    wakeup_channel_(std::make_unique<Channel>(this, wakeup_fd_)),
    timer_queue_((std::make_unique<TimerQueue>(this))) {
    const void* this_address = static_cast<const void*>(this);
    std::stringstream str_this_address;
    str_this_address << this_address;
    LOG_INFO("eventLoop:" + str_this_address.str() + " created in thread:" + std::to_string(thread_id_));
    wakeup_channel_->SetReadCallback(std::bind(&EventLoop::HandleRead, this));
    wakeup_channel_->EnableReading();
    UpdateChannel(wakeup_channel_.get());
    signal(SIGPIPE, SIG_IGN);
}

void EventLoop::Loop() {
    if (looping_) {
        LOG_WARN("eventLoop " + std::to_string(thread_id_) + " is looping");
    }
    IsInLoopThread();
    looping_ = true;
    quit_    = false;
    // timer_queue->Get_expired();
    while (!quit_) {
        channels_.clear();
        poller_return_time = poller_->Poll(poll_timeout_ms, &channels_);
        event_handling_    = true;
        for (Channel* channel : channels_) {
            active_channel_ = channel;
            active_channel_->HandleEvent(poller_return_time);
        }
        event_handling_ = false;
        active_channel_ = nullptr;
        RunPendingFunction();
    }
    looping_ = false;
}

void EventLoop::Quit() {
    quit_                    = true;
    const void* this_address = static_cast<const void*>(this);
    std::stringstream str_this_address;
    str_this_address << this_address;
    LOG_INFO("eventLoop:" + str_this_address.str() + "  earse in thread:" + std::to_string(thread_id_));
    if (!IsInLoopThread()) { Wakeup(); }
}

timer::TimeStamp EventLoop::GetReturnTime() const {
    return poller_return_time;
}

void EventLoop::RunAt(const timer::TimeStamp& time, Function func) {
    IsInLoopThread();
    timer_queue_->AddTimer(std::move(func), time, 0.0);
}

void EventLoop::RunAfter(double delay, Function func) {
    timer::TimeStamp time(timer::TimeStamp::TimeStampAdd(timer::Clock::NowtimeMs(), delay));
    RunAt(time, std::move(func));
}

void EventLoop::RunEvery(double interval, Function func) {
    timer::TimeStamp time(timer::TimeStamp::TimeStampAdd(timer::Clock::NowtimeMs(), interval));
    timer_queue_->AddTimer(std::move(func), time, interval);
}

void EventLoop::UpdateChannel(Channel* channel) {
    if (IsInLoopThread()) {
        poller_->UpdateChannel(channel);
    }
}

void EventLoop::RemoveChannel(Channel* channel) {
    if (IsInLoopThread()) {
        if (event_handling_) {
            // if(active_channel_ != channel)
        }
        poller_->RemoveChannel(channel);
    }
}

bool EventLoop::IsInLoopThread() const {
    if (!(thread_id_ == gettid())) {
        LOG_WARN("eventLoop thread:" + std::to_string(thread_id_)
                 + " not current, Should be: " + std::to_string(gettid()));
        return false;
    }
    return true;
}

void EventLoop::QueueInLoop(Function func) {
    {
        std::lock_guard<std::mutex> lk(mtx_);
        pending_functions.push_back(std::move(func));
    }
    if (!IsInLoopThread() || calling_pending_) { Wakeup(); }
}

void EventLoop::RunInLoop(Function func) {
    if (IsInLoopThread()) {
        func();
    } else {
        QueueInLoop(func);
    }
}

void EventLoop::Wakeup() {
    ssize_t tmp = 1;
    ssize_t ret = write(wakeup_fd_, &tmp, sizeof(tmp));
    if (ret != sizeof(tmp)) {
        LOG_ERROR("writes to " + std::to_string(wakeup_fd_) + " " + std::to_string(ret) + " bytes, should be 8");
    }
}

EventLoop::~EventLoop() {
    close(wakeup_fd_);
}