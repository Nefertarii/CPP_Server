#include "logger.h"
#include "../Base/exception.h"
#include "../Base/Timer/clock.h"

using namespace Wasi;
using namespace Wasi::Log;

void Logger::Log(LogMsg& logmsg) {
    std::lock_guard<std::mutex> lk(mtx);
    for (auto it : sinks) {
        it->Logger(logmsg);
    }
    if (String_to_Level(logmsg.Get_level()) == flush_level) {
        for (auto it : sinks) {
            it->Flush();
        }
    }
}

void Logger::Splice(std::string& file, std::string func, int line) {
    file += ":";
    file += std::to_string(line);
    file += " ";
    file += func;
    file += "()";
}

Logger::Logger(std::string name_) :
    mtx(),
    name(name_),
    error_handler(nullptr),
    sinks(),
    flush_level(LogLevel::NONE) {}

Logger::Logger(std::string name_, SinkPtr sink_) :
    mtx(),
    name(name_),
    error_handler(nullptr),
    sinks(),
    flush_level(LogLevel::NONE) {
    sinks.push_back(sink_);
}

Logger::Logger(std::string name_, SinkInitList sinks_) :
    mtx(),
    name(name_),
    error_handler(nullptr),
    sinks(sinks_.begin(), sinks_.end()),
    flush_level(LogLevel::NONE) {}

Logger::Logger(std::string name_, SinkIt beg, SinkIt end) :
    mtx(),
    name(name_),
    error_handler(nullptr),
    sinks(beg, end),
    flush_level(LogLevel::NONE) {}

void Logger::Debug(std::string detail, int tid, std::string file,
                   std::string func, int line) {
    if (!file.empty()) { Splice(file, func, line); }
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::DBG,
                   detail, tid, file);
    Log(tmp_log);
}

void Logger::Info(std::string detail, int tid, std::string file,
                  std::string func, int line) {
    if (!file.empty()) { Splice(file, func, line); }
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::INFO,
                   detail, tid, file);
    Log(tmp_log);
}

void Logger::Warning(std::string detail, int tid, std::string file,
                     std::string func, int line) {
    if (!file.empty()) { Splice(file, func, line); }
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::WARN,
                   detail, tid, file);
    Log(tmp_log);
}

void Logger::Error(std::string detail, int tid, std::string file,
                   std::string func, int line) {
    if (!file.empty()) { Splice(file, func, line); }
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::ERR,
                   detail, tid, file);
    Log(tmp_log);
}

void Logger::Critical(std::string detail, int tid, std::string file,
                      std::string func, int line) {
    if (!file.empty()) { Splice(file, func, line); }
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::CRITICAL,
                   detail, tid, file);
    Log(tmp_log);
}

void Logger::Fatal(std::string detail, int tid, std::string file,
                   std::string func, int line) {
    if (!file.empty()) { Splice(file, func, line); }
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::FATAL,
                   detail, tid, file);
    Log(tmp_log);
}

size_t Logger::Size() { return sinks.size(); }

void Logger::Flush() {
    try {
        for (auto it : sinks) {
            it->Flush();
        }
    } catch (const Exception& e) {
        Error(e.What());
        if (error_handler) {
            error_handler();
        }
    }
}

void Logger::Flush_on(LogLevel level) { flush_level = level; }

void Logger::Push_back(SinkPtr sink_) {
    Flush();
    std::lock_guard<std::mutex> lk(mtx);
    sinks.push_back(sink_);
}

void Logger::Remove(uint i) {
    Flush();
    std::lock_guard<std::mutex> lk(mtx);
    if (i > sinks.size()) { return; }
    sinks.erase(sinks.begin() + i);
}

void Logger::Set_formatter(LogFormat format) {
    for (auto it : sinks) {
        it->Set_format(format);
    }
}

void Logger::Set_error_handler(ErrorHandler func) { error_handler = func; }

Logger::~Logger() { Flush(); }