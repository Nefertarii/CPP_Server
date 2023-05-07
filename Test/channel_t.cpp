#include "Include/Logger/logger.h"
#include "Include/Eventloop/channel.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Timer/clock.h"

#include <functional>
#include <map>
#include <iostream>
#include <cstring>

#include <stdio.h>
#include <unistd.h>
#include <sys/timerfd.h>

using namespace wasi;
using namespace wasi::loop;

void print(std::string msg) {
    static std::map<std::string, timer::TimeStamp> lasts;
    timer::TimeStamp& last = lasts[msg];
    timer::TimeStamp now   = timer::Clock::NowtimeMs();
    std::cout << std::to_string(now.GetTime()) << " msg:" << msg << " delay:"
              << std::to_string(timer::TimeStamp::TimeStampDiff(now, last));
    last = now;
}

namespace wasi {
namespace net {
namespace detail {
int CreateTimerfd() {
    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    return timerfd;
}

void ReadTimerfd(int timerfd, timer::TimeStamp now) {
    long remaining;
    ssize_t ret = read(timerfd, &remaining, sizeof(remaining));
    if (ret != sizeof(remaining)) {
        LOG_ERROR("Handle read " + std::to_string(ret) + " bytes, Should be 8.");
    }
}
}
}
} // namespace wasi::net::detail

using TimerCallback = std::function<void()>;

// Use relative time, immunized to wall clock changes.
class PeriodicTimer {
public:
    PeriodicTimer(EventLoop* loop, double interval, const TimerCallback& cb) :
        loop_(loop),
        timerfd_(wasi::net::detail::CreateTimerfd()),
        timerfdChannel_(loop, timerfd_),
        interval_(interval),
        cb_(cb) {
        timerfdChannel_.SetReadCallback(
            std::bind(&PeriodicTimer::handleRead, this));
        timerfdChannel_.EnableReading();
    }

    void start() {
        struct itimerspec spec;
        bzero(&spec, sizeof spec);
        spec.it_interval = toTimeSpec(interval_);
        spec.it_value    = spec.it_interval;
        int ret          = ::timerfd_settime(timerfd_, 0 /* relative timer */, &spec, NULL);
        if (ret) {
            LOG_INFO("timerfd_settime()");
        }
    }

    ~PeriodicTimer() {
        timerfdChannel_.DisableReading();
        timerfdChannel_.DisableWriting();
        timerfdChannel_.Remove();
        ::close(timerfd_);
    }

private:
    void handleRead() {
        loop_->IsInLoopThread();
        wasi::net::detail::ReadTimerfd(timerfd_, timer::Clock::NowtimeMs());
        if (cb_)
            cb_();
    }

    static struct timespec toTimeSpec(double seconds) {
        struct timespec ts;
        bzero(&ts, sizeof ts);
        const int64_t kNanoSecondsPerSecond = 1000000000;
        const int kMinInterval              = 100000;
        int64_t nanoseconds                 = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);
        if (nanoseconds < kMinInterval)
            nanoseconds = kMinInterval;
        ts.tv_sec  = static_cast<time_t>(nanoseconds / kNanoSecondsPerSecond);
        ts.tv_nsec = static_cast<long>(nanoseconds % kNanoSecondsPerSecond);
        return ts;
    }

    EventLoop* loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    const double interval_; // in seconds
    TimerCallback cb_;
};

int main(int argc, char* argv[]) {
    log::Logger::Init();
    LOG_INFO("Try adjusting the wall clock, see what happens");
    EventLoop loop;
    PeriodicTimer timer(&loop, 1, std::bind(print, "PeriodicTimer"));
    timer.start();
    loop.RunEvery(1, std::bind(print, "EventLoop::runEvery"));
    loop.Loop();
}