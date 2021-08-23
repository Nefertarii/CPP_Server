#include "Gtime.h"

Ms Timer::nowtime_ms()
{
    Time<Ms> now_ms = std::chrono::time_point_cast<Ms>(Clock::now());
    Ms time_ms = now_ms.time_since_epoch();
    return time_ms;
}
Sec Timer::nowtime_sec()
{
    Time<Sec> now_sec = std::chrono::time_point_cast<Sec>(Clock::now());
    Sec time_sec = now_sec.time_since_epoch();
    return time_sec;
}

Timer::Timer()
{
    begintime_ms = nowtime_ms();
    begintime_sec = nowtime_sec();
}
long Timer::Nowtime_ms()
{
    Ms time_ms = nowtime_ms();
    return time_ms.count();
}
long Timer::Nowtime_sec()
{
    Sec time_sec = nowtime_sec();
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
    Sec time_sec = nowtime_sec();
    long time_long = time_sec.count() - begintime_sec.count();
    std::string time_str = std::to_string(time_long);
    return time_str;
}
