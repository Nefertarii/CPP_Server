#include "Head/clock.h"

using namespace Wasi::Time;

Clock::Clock():create_time(Nowtime_us()) {}

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
    char temp[80];
    long time_sec = time.Microseconds_since_epoch() / TimeStamp::microseconds_per_second;
    struct tm* time_tm = localtime(&time_sec);
    strftime(temp, 80, "Data:%a, %b %m %Y %H:%M:%S GMT\n", time_tm);
    std::string str = temp;
    return str;
}