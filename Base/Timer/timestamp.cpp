#include "timestamp.h"
#include <algorithm>

using namespace Wasi::Time;

TimeStamp::TimeStamp() :
    time(0) {
    /*TimePoint<Us> time_us = std::chrono::time_point_cast<Ms>(HighResClock::now());
    time = time_us.time_since_epoch().count();*/
}

TimeStamp::TimeStamp(long microseconds) :
    time(microseconds) {}

long TimeStamp::Microseconds_since_epoch() { return time; }

void TimeStamp::Swap(TimeStamp& other) {
    std::swap(time, other.time);
}

bool TimeStamp::Valid() const {
    return time > 0;
}

TimeStamp TimeStamp::Invalid() { return TimeStamp(); }
