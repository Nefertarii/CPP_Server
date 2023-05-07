#include "Include/Timer/clock.h"

using namespace wasi::timer;

Clock::Clock() :
    create_time_(NowtimeMs()) {}

long Clock::RuntimeUs() {
    long now_ms = NowtimeUs();
    return now_ms - create_time_.GetTime() * TimeStamp::microseconds_per_milliseconds;
}

long Clock::RuntimeMs() {
    return RuntimeUs() / TimeStamp::microseconds_per_milliseconds;
}

long Clock::RuntimeSec() {
    return RuntimeUs() / TimeStamp::microseconds_per_second;
}

long Clock::NowtimeUs() {
    Timer_Point<Timer_Us> now_us = std::chrono::time_point_cast<Timer_Us>(Timer_Clock::now());
    return now_us.time_since_epoch().count();
}

long Clock::NowtimeMs() { return NowtimeUs() / TimeStamp::microseconds_per_milliseconds; }

long Clock::NowtimeSec() { return NowtimeUs() / TimeStamp::microseconds_per_second; }

std::string Clock::ToString(TimeStamp time) {
    long t_sec = time.GetTime() / TimeStamp::microseconds_per_milliseconds;
    return ToStringSec(t_sec);
}

std::string Clock::ToStringSec(long timestamp_sec, std::string format) {
    char temp[40];
    struct tm* time_tm = localtime(&timestamp_sec);
    strftime(temp, 40, format.c_str(), time_tm);
    std::string str_time = temp;
    return str_time;
}

std::string Clock::ToStringMs(long timestamp_ms, std::string format) {
    long t_sec       = timestamp_ms / TimeStamp::millisecond_per_second;
    long t_msec      = timestamp_ms % TimeStamp::millisecond_per_second;
    std::string time = ToStringSec(t_sec, format);
    time += '.';
    time += std::to_string(t_msec);
    return time;
}

std::string Clock::ToStringUs(long timestamp_us, std::string format) {
    long t_sec       = timestamp_us / TimeStamp::microseconds_per_second;
    long t_usec      = timestamp_us % TimeStamp::microseconds_per_second;
    std::string time = ToStringSec(t_sec, format);
    time += '.';
    time += std::to_string(t_usec);
    return time;
}
