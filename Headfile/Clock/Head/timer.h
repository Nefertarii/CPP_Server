#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>
#include <string>

using Ulong = unsigned long;
using Us = std::chrono::microseconds;
using Ms = std::chrono::milliseconds;
using Sec = std::chrono::seconds;
using SysClock = std::chrono::system_clock;
using HighResClock = std::chrono::high_resolution_clock;
using SteadyClock = std::chrono::steady_clock;
template <typename Rep, typename Period>
using Duration = std::chrono::duration<Rep, Period>;
template <typename type>
using Time = std::chrono::time_point<HighResClock, type>;

class Timer {
private:
    Ms create_time_ms;
    Sec create_time_sec;
    SteadyClock clock;
    Ms Now_time_ms_();
    Sec Now_time_sec_();
public:
    Timer();
    long Now_time_ms();
    long Now_time_sec();
    long Run_time_ms();
    long Run_time_sec();
    std::string Sec_to_string(long time);
    //Set_time();
    void Reset();
    ~Timer() {};
};

#endif