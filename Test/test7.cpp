#include "../Class/exception.h"
#include "../Log/Base/Head/fontcolor.h"
#include "../Log/Base/Head/loglevel.h"
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sys/time.h>
#include <unistd.h>

using namespace std;
using namespace Wasi;
using namespace Wasi::Log;
using Us           = std::chrono::microseconds;
using Ms           = std::chrono::milliseconds;
using Sec          = std::chrono::seconds;
using SysClock     = std::chrono::system_clock;
using HighResClock = std::chrono::high_resolution_clock;
using SteadyClock  = std::chrono::steady_clock;
template <typename Rep, typename Period>
using Duration = std::chrono::duration<Rep, Period>;
template <typename type>
using TimePoint = std::chrono::time_point<HighResClock, type>;

void func1() {
    throw Exception();
}

string Time(long timestamp_ms, const char* time_format) {
    long timestamp_sec = timestamp_ms / 1000;
    int time_ms        = timestamp_ms % 1000;
    char time[40];
    struct tm* time_tm = localtime(&timestamp_sec);
    strftime(time, 40, time_format, time_tm);
    std::string str_time = time;
    return str_time + "." + std::to_string(time_ms) + "\n";
}

void func2() {
    const char* time_format = "%Y-%m-%d %H:%M:%S";
    TimePoint<Ms> now_ms    = std::chrono::time_point_cast<Ms>(HighResClock::now());
    long timestamp          = now_ms.time_since_epoch().count();
    cout << Time(timestamp, time_format);
}

int main() {
    func2();
    const char* file = __FILE__;
    int line         = __LINE__;
    const char* func = __FUNCTION__;
    // try {
    //     func1();
    // } catch (const Exception& e) {
    //     cerr << e.What() << '\n';
    // }
    // fstream file;
    // file.open("test.txt", ios::binary | ios::out | ios::ate);
    // if (file.is_open()) {
    //    cout << "open\n";
    //} else {
    //    cout << strerror(errno) << "\n";
    //}
    // std::cout << file.write("12345", 5) << "\n";
    // if (file.bad()) {
    //    cout << "write fail\n";
    //}
    // file.close();
    // fstream* file2 = &file;
    // file2->open("test.txt", ios::binary | ios::in);
    // std::string str;
    // char str2[1024];
    // while (file2->getline(str2, sizeof(str2))) {
    //    cout << str2 << "\n";
    //}

    // func1();
    //    std::cout << "RESET      " << RESET << "hello" << RESET << "\n";
    //    std::cout << "BLACK      " << BLACK << "hello" << RESET << "\n";
    //    std::cout << "RED        " << RED << "hello" << RESET << "\n";
    //    std::cout << "GREEN      " << GREEN << "hello" << RESET << "\n";
    //    std::cout << "YELLOW     " << YELLOW << "hello" << RESET << "\n";
    //    std::cout << "BLUE       " << BLUE << "hello" << RESET << "\n";
    //    std::cout << "MAGENTA    " << MAGENTA << "hello" << RESET << "\n";
    //    std::cout << "CYAN       " << CYAN << "hello" << RESET << "\n";
    //    std::cout << "WHITE      " << WHITE << "hello" << RESET << "\n";
    //    std::cout << "BOLDBLACK  " << BOLDBLACK << "hello" << RESET << "\n";
    //    std::cout << "BOLDRED    " << BOLDRED << "hello" << RESET << "\n";
    //    std::cout << "BOLDGREEN  " << BOLDGREEN << "hello" << RESET << "\n";
    //    std::cout << "BOLDYELLOW " << BOLDYELLOW << "hello" << RESET << "\n";
    //    std::cout << "BOLDBLUE   " << BOLDBLUE << "hello" << RESET << "\n";
    //    std::cout << "BOLDMAGENTA" << BOLDMAGENTA << "hello" << RESET << "\n";
    //    std::cout << "BOLDCYAN   " << BOLDCYAN << "hello" << RESET << "\n";
    //    std::cout << "BOLDWHITE  " << BOLDWHITE << "hello" << RESET << "\n";

    //    cout << "Line 1...";
    //    usleep(500000);
    //    cout << "\nLine 2" << endl;
    //    cout << "Line 3" << endl;
    //    return 0;
}