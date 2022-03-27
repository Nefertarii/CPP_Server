#include "../Class/exception.h"
#include "../Log/Base/Head/fontcolor.h"
#include "../Log/Sink/Head/filesink.h"
#include "../Log/Sink/Head/stdsink.h"
#include "../Timer/Head/clock.h"
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
using namespace Wasi::Time;

void func1(string& name) { cout << "before open " << name << "\n"; }

void func2(string& name, fstream* filestream) {
    cout << "after open " << name << " fstream: " << filestream << "\n";
}

void func3(string& name, fstream* filestream) {
    cout << "before close " << name << " fstream: " << filestream << "\n";
}

void func4(string& name) { cout << "after close " << name << "\n"; }

void T_filehandler() {
    FileEvents file_events;
    file_events.before_open  = func1;
    file_events.after_open   = func2;
    file_events.before_close = func3;
    file_events.after_close  = func4;
    FileHandler file_handler(file_events);
    file_handler.Open("test.txt", false);
    file_handler.Write("12345");
    cout << file_handler.Get_file_size() << "\n";
    cout << file_handler.Get_file_name() << "\n";
    file_handler.Close();
}

void T_loglevel() {
    vector<LogLevel> vec;
    vec.push_back(LogLevel::UNINITIALIZED);
    vec.push_back(LogLevel::NONE);
    vec.push_back(LogLevel::DBG);
    vec.push_back(LogLevel::INFO);
    vec.push_back(LogLevel::WARN);
    vec.push_back(LogLevel::ERR);
    vec.push_back(LogLevel::CRITICAL);
    vec.push_back(LogLevel::FATAL);
    for (auto i : vec) {
        std::string str = Level_to_string(i);
        LogLevel level  = String_to_Level(str);
        cout << "str:" << str << " level:" << (int)level << "\n";
    }
}

void func5(LogMsg msg) {
    std::cout << "Thread_id:" << msg.Get_thread_id() << " ";
    std::cout << "Location:" << msg.Get_source_location() << " ";
    std::cout << "Date:" << msg.Get_date() << " ";
    std::cout << "Level:" << msg.Get_level() << " ";
    std::cout << "Detail:" << msg.Get_detail() << "\n";
}

void T_logmsg() {
    LogMsg msg1("[2022/03/25 16:42:28.100][debug]test msg1\n");
    string data   = "2022/03/25 21:13:25.123";
    string level  = "warn";
    string detail = "test msg2";
    LogMsg msg2(data, level, detail);
    LogMsg msg3("2022/03/25 21:13:25.321", "warn", "test msg3");
    LogMsg msg4(Clock::Nowtime_ms(), LogLevel::INFO, "test msg4");
    LogMsg msg5;
    msg5.Get_date()   = Clock::Nowtime_ms();
    msg5.Get_level()  = "warn";
    msg5.Get_detail() = "test msg5";
    func5(msg1);
    func5(msg2);
    func5(msg3);
    func5(msg4);
    func5(msg5);
}

void T_formatter() {
    LogFormatter formatter;
    LogMsg msg1("[2022/03/25 16:42:28.100][debug]test msg1\n");
    string data   = "2022/03/25 21:13:25.123";
    string level  = "warn";
    string detail = "test msg2";
    LogMsg msg2(data, level, detail);
    LogMsg msg3("2022/03/25 21:13:25.123", "error", "test msg3");
    LogMsg msg4(Clock::Nowtime_ms(), LogLevel::FATAL, "test msg4");
    std::cout << formatter.Format(msg1);
    std::string str1;
    formatter.Format(msg2, str1);
    std::cout << str1;
    msg3.Format(formatter.Format(msg3));
    std::cout << msg3.Output();
    std::cout << formatter.Format(msg4) << "\n";
}

void T_stdsink() {
}

void T_filesink() {
}

int main() {
}