#include "Head/logger.h"
#include "../Class/exception.h"
#include "../Timer/Head/clock.h"

using namespace Wasi;
using namespace Wasi::Log;

void Logger::Log(LogMsg& logmsg) {
    if (String_to_Level(logmsg.Get_level()) == flush_level) {
        for (auto it : sinks) {
            it->Flush();
        }
    }
    for (auto it : sinks) {
        it->Logger(logmsg);
    }
}

Logger::Logger(std::string name_) :
    name(name_),
    error_handler(nullptr),
    sinks(),
    flush_level(LogLevel::NONE) {}

Logger::Logger(std::string name_, SinkPtr sink_) :
    name(name_),
    error_handler(nullptr),
    sinks(),
    flush_level(LogLevel::NONE) {
    sinks.push_back(sink_);
}

Logger::Logger(std::string name_, SinkInitList sinks_) :
    name(name_),
    error_handler(nullptr),
    sinks(sinks_.begin(), sinks_.end()),
    flush_level(LogLevel::NONE) {}

Logger::Logger(std::string name_, SinkIt beg, SinkIt end) :
    name(name_),
    error_handler(nullptr),
    sinks(beg, end),
    flush_level(LogLevel::NONE) {}

void Logger::Debug(std::string detail, int tid, std::string source) {
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::DBG,
                   detail, tid, source);
    Log(tmp_log);
}

void Logger::Info(std::string detail, int tid, std::string source) {
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::INFO,
                   detail, tid, source);
    Log(tmp_log);
}

void Logger::Warning(std::string detail, int tid, std::string source) {
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::WARN,
                   detail, tid, source);
    Log(tmp_log);
}

void Logger::Error(std::string detail, int tid, std::string source) {
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::ERR,
                   detail, tid, source);
    Log(tmp_log);
}

void Logger::Critical(std::string detail, int tid, std::string source) {
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::CRITICAL,
                   detail, tid, source);
    Log(tmp_log);
}

void Logger::Fatal(std::string detail, int tid, std::string source) {
    LogMsg tmp_log(Time::Clock::Nowtime_ms(), LogLevel::FATAL,
                   detail, tid, source);
    Log(tmp_log);
}

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

void Logger::Flush_on(LogLevel level) {
    flush_level = level;
}

void Logger::Set_formatter(LogFormat format) {
    for (auto it : sinks) {
        it->Set_format(format);
    }
}

void Logger::Set_error_handler(ErrorHandler func) {
    error_handler = func;
}

Logger::~Logger() {
    Flush();
}