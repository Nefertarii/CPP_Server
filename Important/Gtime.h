#ifndef GTIME_H_
#define GTIME_H_

#include <chrono>
#include <ctime>
#include <signal.h>
#include <string>
#include <unistd.h>

using Ms = std::chrono::milliseconds;
using Sec = std::chrono::seconds;
using Clock = std::chrono::system_clock;
//using Timepoint = std::chrono::time_point<Clock>;
template <typename type>
using Time = std::chrono::time_point<Clock, type>;

class Timer {
private:
    Ms begintime_ms;
    Sec begintime_sec;

public:
    Timer();
    static Ms Nowtime_Ms();
    static Sec Nowtime_Sec();
    static long Nowtime_ms();
    static long Nowtime_sec();
    static std::string Nowtime_str();
    long Runtime_ms() { return Nowtime_ms() - begintime_ms.count(); }
    long Runtime_sec() { return Nowtime_sec() - begintime_sec.count(); }
    std::string Runtime_str();
    void Reset();
    ~Timer() {}
};

#endif