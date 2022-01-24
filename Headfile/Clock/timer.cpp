#include "Head/timer.h"

Ms Timer::Now_time_ms_() {
    Time<Ms> now_ms = std::chrono::time_point_cast<Ms>(HighResClock::now());
    return now_ms.time_since_epoch();
}

Sec Timer::Now_time_sec_() {
    Time<Sec> now_sec = std::chrono::time_point_cast<Sec>(HighResClock::now());
    return now_sec.time_since_epoch();
}

Timer::Timer() {
    create_time_ms = Now_time_ms_();
    create_time_sec = Now_time_sec_();
}

long Timer::Now_time_ms() {
    Time<Ms> now_ms = std::chrono::time_point_cast<Ms>(HighResClock::now());
    return now_ms.time_since_epoch().count();
}

long Timer::Now_time_sec() {
    Time<Sec> now_sec = std::chrono::time_point_cast<Sec>(HighResClock::now());
    return now_sec.time_since_epoch().count();
}

long Timer::Run_time_ms() {
    Ms time_ms = Now_time_ms_();
    return time_ms.count() - create_time_ms.count();
}

long Timer::Run_time_sec() {
    Sec time_sec = Now_time_sec_();
    return time_sec.count() - create_time_sec.count();
}

std::string Timer::Sec_to_string(long time) {
    char temp[80];
    struct tm *time_tm = localtime(&time);
    strftime(temp, 80, "Data:%a, %b %m %Y %H:%M:%S GMT", time_tm);
    std::string str = temp;
    return str;
}

void Timer::Reset() {
    create_time_ms = Now_time_ms_();
    create_time_sec = Now_time_sec_();
}