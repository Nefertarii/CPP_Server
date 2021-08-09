#include <ctime>
#include <chrono>
#include <signal.h>

using msTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration>;
using Clock = std::chrono::system_clock;

//using Ms = std::chrono::milliseconds;
//using Sec = std::chrono::seconds;
//template<class Duration>
//using TimePoint = std::chrono::time_point<Clock, Duration>;
//TimePoint<Ms> time_ms;
//inline void print_ms(const msTime &time_point)
//{ std::cout << time_point.time_since_epoch().count() << " ms\n"; }

class Timer
{
private:
    Clock clock;
    msTime begintime;

public:
    Timer();
    msTime createTime();
    void nowTime(char *time_c);
    void runTime(int *time_i);
    ~Timer();
};
Timer::Timer()
{
    begintime = clock.now();
}

msTime Timer::createTime() { return begintime; }

void Timer::nowTime(char *time_c)
{
    msTime nowtime(std::chrono::system_clock::now());
    const time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm *time_tm = localtime(&time);
    strftime(time_c, 80, "Data:%a, %b %m %Y %H:%M:%S GMT\n", time_tm);
}

void Timer::runTime(int *time_i)
{
    msTime endtime = clock.now();
    //time_i = begintime - endtime;
}

Timer::~Timer()
{

}
