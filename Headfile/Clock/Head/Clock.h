#ifndef CLOCK_H_
#define CLOCK_H_

#include <iostream>
#include <chrono>

using Ulong = unsigned long;
using Us = std::chrono::microseconds;
using Ms = std::chrono::milliseconds;
using Sec = std::chrono::seconds;
using SysClock = std::chrono::system_clock;
using HighResClock = std::chrono::high_resolution_clock;
using SteadyClock = std::chrono::steady_clock;
template<typename Rep, typename Period>
using Duration = std::chrono::duration<Rep, Period>;
template<typename Clock, typename Duration>
using TimePoint = std::chrono::time_point<Clock, Duration>;

class Clock {
private:
    Ms create_time_ms;
    Us create_time_us;
    TimePoint<Ms,> create_time;
public:
    
};

#endif