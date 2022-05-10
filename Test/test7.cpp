#include "../Class/exception.h"
#include "../Log/Base/Head/filehandler.h"
#include "../Log/Head/logging.h"
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
#include <thread>
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
    StdSink stdsink;
    LogMsg msg1("[2022/03/25 16:42:28.100][debug]test msg1\n");
    string data   = "2022/03/25 21:13:25.123";
    string level  = "warn";
    string detail = "test msg2";
    LogMsg msg2(data, level, detail);
    stdsink.Logger(msg1);
    stdsink.Logger(msg2);
}

void T_filesink() {
    FileSink filesink("test.txt");
    LogMsg msg1;
    msg1.Get_level()  = "debug";
    msg1.Get_detail() = "file test";
    for (int i = 0; i < 50; i++) {
        msg1.Get_date() = Clock::Nowtime_ms();
        this_thread::sleep_for(Ms(10));
        filesink.Logger(msg1);
    }
}

void T_logger() {
    Logger logger1("logger1", make_shared<StdSink>());
    logger1.Flush_on(LogLevel::CRITICAL);
    logger1.Debug("testlog1");
    logger1.Info("testlog2");
    logger1.Warning("testlog3");
    logger1.Error("testlog4");
    logger1.Critical("testlog5");
    logger1.Fatal("testlog6");

    SinkPtr stdsinkptr  = make_shared<StdSink>();
    SinkPtr filesinkptr = make_shared<FileSink>("test.txt");
    Logger logger2("logger2", {stdsinkptr, filesinkptr});
    LogFormat fmt;
    fmt.print_thread_id       = true;
    fmt.print_source_location = true;
    logger2.Set_formatter(fmt);
    logger2.Debug("testlog1", gettid(), __FILE__, __FUNCTION__, __LINE__);
    logger2.Info("testlog2", gettid(), __FILE__, __FUNCTION__, __LINE__);
    logger2.Warning("testlog3", gettid(), __FILE__, __FUNCTION__, __LINE__);
    logger2.Error("testlog4", gettid(), __FILE__, __FUNCTION__, __LINE__);
    logger2.Critical("testlog5", gettid(), __FILE__, __FUNCTION__, __LINE__);
    logger2.Fatal("testlog6", gettid(), __FILE__, __FUNCTION__, __LINE__);

    logger2.Remove(0);
    std::cout << "remove\n";
    logger2.Debug("testlog1", gettid(), __FILE__, __FUNCTION__, __LINE__);
    logger2.Info("testlog2", gettid(), __FILE__, __FUNCTION__, __LINE__);

    SinkPtr stdsinkptr2 = make_shared<StdSink>();
    logger2.Push_back(stdsinkptr2);
    std::cout << "push\n";

    logger2.Debug("testlog1", gettid(), __FILE__, __FUNCTION__, __LINE__);
    logger2.Info("testlog2", gettid(), __FILE__, __FUNCTION__, __LINE__);
}

void T_logging() {
    Debug("123");
    Debug("123", gettid());
    Debug("123", gettid(), __FILE__, __FUNCTION__);
    Debug("123", gettid(), __FILE__, __FUNCTION__, __LINE__);
    Info("123");
    Info("123", gettid());
    Info("123", gettid(), __FILE__, __FUNCTION__);
    Info("123", gettid(), __FILE__, __FUNCTION__, __LINE__);
    Warning("123");
    Warning("123", gettid());
    Warning("123", gettid(), __FILE__, __FUNCTION__);
    Warning("123", gettid(), __FILE__, __FUNCTION__, __LINE__);
    Error("123");
    Error("123", gettid());
    Error("123", gettid(), __FILE__, __FUNCTION__);
    Error("123", gettid(), __FILE__, __FUNCTION__, __LINE__);
    Critical("123");
    Critical("123", gettid());
    Critical("123", gettid(), __FILE__, __FUNCTION__);
    Critical("123", gettid(), __FILE__, __FUNCTION__, __LINE__);
    Fatal("123");
    Fatal("123", gettid());
    Fatal("123", gettid(), __FILE__, __FUNCTION__);
    Fatal("123", gettid(), __FILE__, __FUNCTION__, __LINE__);
}

void T_speed() {
    long begin = Clock::Nowtime_ms();
    for (int i = 0; i != 50000; i++) {
        Debug("123");
    }
    long end = Clock::Nowtime_ms();
    std::cout << "global log Using " << end - begin << " ms\n";
    sleep(5);

    begin = Clock::Nowtime_ms();
    Logger logger("logger", make_shared<StdSink>());
    for (int i = 0; i != 50000; i++) {
        logger.Debug("123");
    }
    end = Clock::Nowtime_ms();
    sleep(1);
    std::cout << endl;
    std::cout << "logger Using " << end - begin << " ms\n";
}

void T_thread_log(int j) {
    for (int i = 0; i != 5000; i++) {
        LOG_INFO("test" + to_string(j));
    }
}

void T_global_log() {
    long begin = Clock::Nowtime_ms();
    for (int i = 0; i != 5; i++) {
        LOG_INFO("test0");
    }
    long end = Clock::Nowtime_ms();
    std::cout << "global std log Using " << end - begin << " ms\n";
    begin = Clock::Nowtime_ms();

    Change_default_logger(std::make_shared<Log::FileSink>("test.log"));
    cout << default_logger->Size();
    std::cout << "change log sink\n";

    for (int i = 0; i != 500; i++) {
        LOG_INFO("test5");
    }
    thread t1(T_thread_log, 1);
    thread t2(T_thread_log, 2);
    thread t3(T_thread_log, 3);
    thread t4(T_thread_log, 4);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    end = Clock::Nowtime_ms();
    std::cout << "global file log Using " << end - begin << " ms\n";
}

int main() {
    // T_stdsink();
    // T_filehandler();
    // T_filesink();
    // T_logger();
    // T_logging();
    // T_speed();
    T_global_log();
}