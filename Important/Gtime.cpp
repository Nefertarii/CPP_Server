#include "Gtime.h"


Timer::Timer()
{
    begintime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
}

Gtime Timer::createTime() { return begintime; }

Gtime Timer::nowTime()
{
    Gtime nowTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
    return nowTime;
}

void Timer::nowTime(char *time_c)
{
    Gtime nowtime = nowTime();
    const time_t time = std::chrono::system_clock::to_time_t(nowtime);
    struct tm *time_tm = localtime(&time);
    strftime(time_c, 80, "Data:%a, %b %m %Y %H:%M:%S GMT\n", time_tm);
}

void Timer::runTime(int *time_i)
{
    Gtime endtime = nowTime();
    time_i = begintime - endtime;
}

Timer::~Timer()
{

}
