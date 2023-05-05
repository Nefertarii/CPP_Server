#include <any>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <sys/time.h>
#include <vector>

using namespace std;

enum class Method {
    INVAILD,
    GET,
    POST,
    HEAD,
    PUT,
    DELETE
};
enum class HttpVersion {
    UNKNOWN,
    HTTP10,
    HTTP11,
    HTTP20
};
enum class HttpCode {
    CODE200 = 200, // 200 OK
    CODE301 = 301, // 301 Moved Permanently
    CODE304 = 304, // 304 Not Modified
    CODE400 = 400, // 400 Bad Request
    CODE403 = 403, // 403 Forbidden
    CODE404 = 404, // 404 Not Found
    CODE411 = 411, // 411 Length Required
    CODE500 = 500, // 500 Internal Server Error
    CODE501 = 501, // 501 Method Not Implemented
};
struct HttpHead {
    std::string content_type;
    std::string content_length;
    std::string content_encoding;
    std::string cache_control;
    std::string date;
    std::string etag;
    std::string location;
    std::string last_modified;
    std::string server;
};

inline std::string Filename(std::string name) {
    return std::string(name.begin() + name.find_last_of('/'), name.end());
}

int flag;            // 11111
Method method;       // 00001
HttpVersion version; // 00010
std::string body;    // 00100
std::string target;  // 01000
std::string path;    // 10000

class Test {
public:
    int i = 0;
    Test(int num) :
        i(num) { cout << "test create " << i << "\n"; }
    ~Test() { cout << "test earse " << i << "\n"; }
};

const int microseconds_per_second       = 1000 * 1000;
const int microseconds_per_milliseconds = 1000;
const int millisecond_per_second        = 1000;
using Us                                = std::chrono::microseconds;
using Ms                                = std::chrono::milliseconds;
using Sec                               = std::chrono::seconds;
using SysClock                          = std::chrono::system_clock;
using HighResClock                      = std::chrono::high_resolution_clock;
using SteadyClock                       = std::chrono::steady_clock;
template <typename Rep, typename Period>
using Duration = std::chrono::duration<Rep, Period>;
template <typename type>
using TimePoint = std::chrono::time_point<HighResClock, type>;

long Nowtime_us() {
    TimePoint<Us> now_us = std::chrono::time_point_cast<Us>(HighResClock::now());
    return now_us.time_since_epoch().count();
}

long Nowtime_ms() { return Nowtime_us() / microseconds_per_milliseconds; }

long Nowtime_sec() { return Nowtime_us() / microseconds_per_second; }

std::string To_string_sec(long timestamp_sec, std::string format) {
    char temp[40];
    struct tm* time_tm = localtime(&timestamp_sec);
    strftime(temp, 40, format.c_str(), time_tm);
    std::string str_time = temp;
    return str_time;
}

std::string To_string_ms(long timestamp_ms, std::string format) {
    long t_sec       = timestamp_ms / millisecond_per_second;
    long t_msec      = timestamp_ms % millisecond_per_second;
    std::string time = To_string_sec(t_sec, format);
    time += '.';
    time += std::to_string(t_msec);
    return time;
}

std::string To_string_us(long timestamp_us, std::string format) {
    long t_sec       = timestamp_us / microseconds_per_second;
    long t_usec      = timestamp_us % microseconds_per_second;
    std::string time = To_string_sec(t_sec, format);
    time += '.';
    time += std::to_string(t_usec);
    return time;
}

int main() {
    // clang-format off
    cout << To_string_sec(Nowtime_sec(), "%b %m %Y %H:%M:%S") << "\n";
    cout << To_string_ms(Nowtime_ms(), "%b %m %Y %H:%M:%S") << "\n";
    cout << To_string_us(Nowtime_us(), "%b %m %Y %H:%M:%S") << "\n";
}
