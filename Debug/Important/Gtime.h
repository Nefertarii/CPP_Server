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

class Timer
{
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





Ms Timer::Nowtime_Ms() {
    Time<Ms> now_ms = std::chrono::time_point_cast<Ms>(Clock::now());
    Ms time_ms = now_ms.time_since_epoch();
    return time_ms;
}

Sec Timer::Nowtime_Sec() {
    Time<Sec> now_sec = std::chrono::time_point_cast<Sec>(Clock::now());
    Sec time_sec = now_sec.time_since_epoch();
    return time_sec;
}

Timer::Timer() {
    begintime_ms = Nowtime_Ms();
    begintime_sec = Nowtime_Sec();
}

long Timer::Nowtime_ms() {
    Ms time_ms = Nowtime_Ms();
    return time_ms.count();
}

long Timer::Nowtime_sec() {
    Sec time_sec = Nowtime_Sec();
    return time_sec.count();
}

std::string Timer::Nowtime_str() {
    char temp[80];
    const time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm *time_tm = localtime(&time);
    strftime(temp, 80, "Data:%a, %b %m %Y %H:%M:%S GMT", time_tm);
    std::string str = temp;
    return str;
}

std::string Timer::Runtime_str() {
    Sec time_sec = Nowtime_Sec();
    long time_long = time_sec.count() - begintime_sec.count();
    std::string time_str = std::to_string(time_long);
    return time_str;
}

void Timer::Reset() {
    begintime_ms = Nowtime_Ms();
    begintime_sec = Nowtime_Sec();
}

#endif