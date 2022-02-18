#include "Head/timer.h"
#include "Head/timestamp.h"

using namespace Wasi::Time;

std::atomic<int> Timer::create_num = 0;

Timer::Timer(std::function<void()> cb, TimeStamp when, double interval_) :
    callback(cb),
    expiration(when),
    interval(interval_),
    repeat(interval > 0.0),
    sequence(create_num++) {}

void Timer::Run() const { callback(); }

void Timer::Restart(TimeStamp now) {
    if (repeat) {
        expiration = Time_stamp_add(now, interval);
    }
    else {
        expiration = TimeStamp::Invalid();
    }
}

bool Timer::Repeat() { return repeat; }

int Timer::Sequence() { return sequence; }

int Timer::Create_num() { return create_num; }

TimeStamp Timer::Expiration() { return expiration; }

/*


            
            ~Timer();
*/