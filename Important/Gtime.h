#ifndef GTIME_H_
#define GTIME_H_

#include <chrono>
#include <ctime>
#include <signal.h>
#include <string>
#include <unistd.h>

using ms = std::chrono::milliseconds;
using sec = std::chrono::seconds;
using Clock = std::chrono::system_clock;
using Timepoint = std::chrono::time_point<Clock>;

template <typename type>
using Time = std::chrono::time_point<Clock, type>;

class Timer
{
private:
    Clock clock;
    Timepoint begintime;
    ms begintime_ms;
    sec begintime_sec;
    ms nowtime_ms();
    sec nowtime_sec();

public:
    Timer();
    long Nowtime_ms();
    long Nowtime_sec();
    std::string Nowtime_str();
    long Runtime_ms() { return Nowtime_ms() - begintime_ms.count(); }
    long Runtime_sec() { return Nowtime_sec() - begintime_sec.count(); }
    std::string Runtime_str();
    ~Timer() {}
};

ms Timer::nowtime_ms()
{
    Time<ms> now_ms = std::chrono::time_point_cast<ms>(Clock::now());
    ms time_ms = now_ms.time_since_epoch();
    return time_ms;
}
sec Timer::nowtime_sec()
{
    Time<sec> now_sec = std::chrono::time_point_cast<sec>(Clock::now());
    sec time_sec = now_sec.time_since_epoch();
    return time_sec;
}

Timer::Timer()
{
    begintime_ms = nowtime_ms();
    begintime_sec = nowtime_sec();
}
long Timer::Nowtime_ms()
{
    ms time_ms = nowtime_ms();
    return time_ms.count();
}
long Timer::Nowtime_sec()
{
    sec time_sec = nowtime_sec();
    return time_sec.count();
}
std::string Timer::Nowtime_str()
{
    char temp[80];
    const time_t time = clock.to_time_t(clock.now());
    struct tm *time_tm = localtime(&time);
    strftime(temp, 80, "Data:%a, %b %m %Y %H:%M:%S GMT", time_tm);
    std::string str = temp;
    return str;
}
std::string Timer::Runtime_str()
{
    sec time_sec = nowtime_sec();
    long time_long = time_sec.count() - begintime_sec.count();
    std::string time_str = std::to_string(time_long);
    return time_str;
}

#endif