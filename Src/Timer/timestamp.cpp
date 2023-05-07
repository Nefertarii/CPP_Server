
#include "Include/Timer/timestamp.h"
#include <algorithm>

using namespace wasi;
using namespace wasi::timer;

const int TimeStamp::microseconds_per_second       = 1000 * 1000;
const int TimeStamp::microseconds_per_milliseconds = 1000;
const int TimeStamp::millisecond_per_second        = 1000;

TimeStamp::TimeStamp() :
    time_(0) {}

TimeStamp::TimeStamp(long millisecond) :
    time_(millisecond) {}

TimeStamp TimeStamp::TimeStampAdd(TimeStamp timestamp, double seconds) {
    long delta = static_cast<long>(seconds * TimeStamp::millisecond_per_second);
    return TimeStamp(timestamp.GetTime() + delta);
}
double TimeStamp::TimeStampDiff(TimeStamp high, TimeStamp low) {
    long difftime = high.GetTime() - low.GetTime();
    return static_cast<double>(difftime) / TimeStamp::millisecond_per_second;
}

void TimeStamp::TimeAdd(double seconds) {
    time_ += static_cast<long>(seconds * TimeStamp::millisecond_per_second);
}

double TimeStamp::TimeDiff(TimeStamp other) {
    long difftime = 0;
    if (other.GetTime() >= time_) {
        difftime = other.GetTime() - time_;
    } else {
        difftime = time_ - other.GetTime();
    }
    return static_cast<double>(difftime) / TimeStamp::millisecond_per_second;
}

// return millisecond timestamp
long TimeStamp::GetTime() { return time_; }

void TimeStamp::Swap(TimeStamp& other) { std::swap(time_, other.time_); }

bool TimeStamp::Valid() const { return time_ > 0; }

TimeStamp TimeStamp::Invalid() { return TimeStamp(); }

bool wasi::timer::operator<(TimeStamp lhs, TimeStamp rhs) {
    return lhs.GetTime() < rhs.GetTime();
}

bool wasi::timer::operator==(TimeStamp lhs, TimeStamp rhs) {
    return lhs.GetTime() == rhs.GetTime();
}