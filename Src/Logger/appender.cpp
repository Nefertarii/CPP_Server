#include "Include/Logger/appender.h"
#include "Include/Timer/clock.h"

using namespace wasi;
using namespace wasi::log;

void Appender::Log(LogMsg& logmsg) {
    if (logmsg.formatted) {
        for (SinkPtr it : sinks_) {
            it->Insert(logmsg.logmsg);
        }
    }
}

bool Appender::Filter(LogLevel level) {
    if ((level & filter_levels_) == 0) {
        return false;
    }
    return true;
}

void Appender::Splice(std::string& file, std::string func, int line) {
    file += ":";
    file += std::to_string(line);
    file += " ";
    file += func;
    file += "()";
}

Appender::Appender(std::string name) :
    mtx_(),
    name_(name),
    error_handler_(nullptr),
    sinks_(),
    formater_(),
    filter_levels_(LogLevel::UINIT) {}

Appender::Appender(std::string name, SinkPtr sink) :
    mtx_(),
    name_(name),
    error_handler_(nullptr),
    sinks_(),
    formater_(),
    filter_levels_(LogLevel::UINIT) {
    sinks_.push_back(sink);
}

Appender::Appender(std::string name, SinkInitList sinks) :
    mtx_(),
    name_(name),
    error_handler_(nullptr),
    sinks_(sinks.begin(), sinks.end()),
    formater_(),
    filter_levels_(LogLevel::UINIT) {}

Appender::Appender(std::string name, SinkIt beg, SinkIt end) :
    mtx_(),
    name_(name),
    error_handler_(nullptr),
    sinks_(beg, end),
    formater_(),
    filter_levels_(LogLevel::UINIT) {}

bool Appender::Log(LogLevel level, std::string content, int tid, std::string file,
                   std::string func, int line) {
    if (Filter(level)) {
        return false;
    }
    LogMsg logmsg;
    logmsg.level   = level;
    logmsg.content = content;
    logmsg.thread  = tid;
    logmsg.date    = timer::Clock::NowtimeMs();
    Splice(file, func, line);
    logmsg.location = file;
    formater_.Format(logmsg);
    Log(logmsg);
    return true;
}

size_t Appender::Size() {
    return sinks_.size();
}

void Appender::Flush() {
    for (auto it : sinks_) {
        it->Flush();
    }
}

void Appender::PushBack(SinkPtr sink_) {
    Flush();
    std::lock_guard<std::mutex> lk(mtx_);
    sinks_.push_back(sink_);
}

bool Appender::Remove(size_t i) {
    Flush();
    std::lock_guard<std::mutex> lk(mtx_);
    if (i > sinks_.size()) { return false; }
    sinks_.erase(sinks_.begin() + i);
    return true;
}

std::vector<std::string> Appender::GetListName() {
    std::vector<std::string> sink_list;
    for (auto it : sinks_) {
        sink_list.push_back(it->GetName());
    }
    return sink_list;
}

void Appender::SetFilterLevel(LogLevel filter) {
    filter_levels_ = filter_levels_ | filter;
}

void Appender::CancelFilterLevel(LogLevel filter) {
    filter_levels_ = filter_levels_ ^ filter;
}

void Appender::SetFormatter(LogFormatter formater) {
    formater_ = formater;
}

void Appender::SetErrorHandler(ErrorHandler func) {
    error_handler_ = func;
}

Appender::~Appender() {
    Flush();
}