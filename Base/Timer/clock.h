#ifndef TIMER_CLOCK_H_
#define TIMER_CLOCK_H_

#include "timestamp.h"
#include <chrono>
#include <string>

namespace Wasi {
namespace Time {

using Us           = std::chrono::microseconds;
using Ms           = std::chrono::milliseconds;
using Sec          = std::chrono::seconds;
using SysClock     = std::chrono::system_clock;
using HighResClock = std::chrono::high_resolution_clock;
using SteadyClock  = std::chrono::steady_clock;
template <typename Rep, typename Period>
using Duration = std::chrono::duration<Rep, Period>;
template <typename type>
using TimePoint = std::chrono::time_point<HighResClock, type>;

class Clock {
private:
    TimeStamp create_time;
    Us Nowtime_us_();

public:
    Clock();
    long Runtime_us();
    long Runtime_ms();
    long Runtime_sec();
    static long Nowtime_us();
    static long Nowtime_ms();
    static long Nowtime_sec();
    static std::string To_string(TimeStamp time); // default use sec
    static std::string To_string_sec(long timestamp_sec, std::string format = "%b %m %Y %H:%M:%S");
    static std::string To_string_ms(long timestamp_ms, std::string format = "%b %m %Y %H:%M:%S");
    static std::string To_string_us(long timestamp_us, std::string format = "%b %m %Y %H:%M:%S");
    // static std::string Ms_to_string(long time);
    // static std::string Us_to_string(long time);
    ~Clock(){};
};

}
} // namespace Wasi::Time

#endif // !TIMER_CLOCK_H_