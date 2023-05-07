#ifndef TIMER_CLOCK_H_
#define TIMER_CLOCK_H_

#include "timestamp.h"
#include <chrono>
#include <string>

namespace wasi {
namespace timer {

using Timer_Us    = std::chrono::microseconds;
using Timer_Ms    = std::chrono::milliseconds;
using Timer_Sec   = std::chrono::seconds;
using Timer_Clock = std::chrono::high_resolution_clock;
template <typename Rep, typename Period>
using Timer_Duration = std::chrono::duration<Rep, Period>;
template <typename Type>
using Timer_Point = std::chrono::time_point<Timer_Clock, Type>;

class Clock {
private:
    TimeStamp create_time_;

public:
    Clock();
    long RuntimeUs();
    long RuntimeMs();
    long RuntimeSec();
    static long NowtimeUs();
    static long NowtimeMs();
    static long NowtimeSec();
    static std::string ToString(TimeStamp time); // default use ToSecString
    static std::string ToStringSec(long timestamp_sec, std::string format = "%b %m %Y %H:%M:%S");
    static std::string ToStringMs(long timestamp_ms, std::string format = "%b %m %Y %H:%M:%S");
    static std::string ToStringUs(long timestamp_us, std::string format = "%b %m %Y %H:%M:%S");
    // static std::string Ms_to_string(long time);
    // static std::string Us_to_string(long time);
    ~Clock(){};
};

}} // namespace wasi::timer

#endif