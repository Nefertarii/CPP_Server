#include "Head/timestamp.h"

using namespace Wasi::Time;

TimeStamp::TimeStamp() : time(0) {
    /*TimePoint<Us> time_us = std::chrono::time_point_cast<Ms>(HighResClock::now());
    time = time_us.time_since_epoch().count();*/
}

TimeStamp::TimeStamp(long microseconds) : time(microseconds) {
}

long TimeStamp::MicroSeconds_since_epoch() { return time; }

TimeStamp::Swap(TimeStamp& other) {
    std::swap(time, other.time);
}

TimeStamp Time_stamp_add(TimeStamp timestamp, double seconds) {
    long addtime = static_cast<long>(seconds * TimeStamp::microseconds_per_second);
    return Timestamp(timestamp.MicroSeconds_since_epoch() + addtime);
}

double Time_stamp_diff(TimeStamp high, TimeStamp low) {
    long difftime = high.MicroSeconds_since_epoch() - low.MicroSeconds_since_epoch();
    return static_cast<double>(diff) / TimeStamp::microseconds_per_second;
}

inline bool operator<(TimeStamp lhs, TimeStamp rhs) {
    return lhs.MicroSeconds_since_epoch() < rhs.MicroSeconds_since_epoch();
}

inline bool operator==(TimeStamp lhs, TimeStamp rhs) {
    return lhs.MicroSeconds_since_epoch() == rhs.MicroSeconds_since_epoch();
}

