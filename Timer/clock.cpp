#include "Head/clock.h"

using namespace Wasi::Time;

Clock::Clock() :
    create_time(Nowtime_us()) {}

long Clock::Runtime_us() {
    long time_us = Nowtime_us();
    return create_time.Microseconds_since_epoch() - time_us;
}

long Clock::Runtime_ms() {
    return Runtime_us() / TimeStamp::microseconds_per_milliseconds;
}

long Clock::Runtime_sec() {
    return Runtime_us() / TimeStamp::microseconds_per_second;
}

long Clock::Nowtime_us() {
    TimePoint<Us> now_us = std::chrono::time_point_cast<Us>(HighResClock::now());
    return now_us.time_since_epoch().count();
}

long Clock::Nowtime_ms() { return Nowtime_us() / TimeStamp::microseconds_per_milliseconds; }

long Clock::Nowtime_sec() { return Nowtime_us() / TimeStamp::microseconds_per_second; }

std::string Clock::To_string(TimeStamp time) {
    long t_sec = time.Microseconds_since_epoch() / TimeStamp::microseconds_per_second;
    return To_string_sec(t_sec, "%b %m %Y %H:%M:%S");
}

std::string TimeStamp::To_string_sec(long timestamp_sec, std::string format) {
    char temp[40];
    struct tm* time_tm = localtime(&timestamp_sec);
    strftime(temp, 40, format.c_str(), time_tm);
    std::string str_time = temp;
    return str_time;
}

std::string TimeStamp::To_string_ms(long timestamp_ms, std::string format) {
    long t_sec       = timestamp_ms / TimeStamp::millisecond_per_second;
    long t_msec      = timestamp_ms % TimeStamp::millisecond_per_second;
    std::string time = To_string_sec(t_sec, format);
    time += '.';
    time += std::to_string(t_msec);
    return time;
}

std::string TimeStamp::To_string_us(long timestamp_us, std::string format) {
    long t_sec       = timestamp_us / TimeStamp::microseconds_per_second;
    long t_usec      = timestamp_us % TimeStamp::microseconds_per_second;
    std::string time = To_string_sec(t_sec, format);
    time += '.';
    time += std::to_string(t_usec);
    return time;
}