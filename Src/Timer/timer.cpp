#include "Include/Timer/timer.h"

using namespace wasi;
using namespace wasi::timer;

std::atomic<int> Timer::create_num_(0);

Timer::Timer(const std::function<void()>& callback, TimeStamp when, double interval) :
    callback_(callback),
    expiration_(when),
    interval_(interval),
    repeat_(interval > 0.0),
    sequence_(create_num_++) {}

void Timer::Run() const { callback_(); }

void Timer::Restart(TimeStamp now) {
    if (repeat_) {
        expiration_ = TimeStamp::TimeStampAdd(now, interval_);
    } else {
        expiration_ = TimeStamp::Invalid();
    }
}

bool Timer::Repeat() { return repeat_; }

int Timer::Sequence() { return sequence_; }

int Timer::CreateNum() { return create_num_; }

TimeStamp Timer::Expiration() { return expiration_; }