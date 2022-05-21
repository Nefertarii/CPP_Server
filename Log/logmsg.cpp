#include "logmsg.h"
#include "../Base/exception.h"
#include <iomanip>

#include <iostream>

using namespace Wasi;
using namespace Wasi::Log;

// Find char stop
std::string LogMsg::Process(std::string& str, char stop) {
    size_t size = str.size();
    if (size == 0) { return std::string(); }
    size_t index = 0;
    while (1) {
        index = str.find_first_of(stop);
        if (index <= size) {
            std::string tmp(str.begin() + 1, str.begin() + index);
            str.erase(str.begin(), str.begin() + index + 1);
            return tmp;
        }
        throw Exception("LogMsg::process() LogMsg " + str + " input error\n");
    }
}

std::string split_date(std::string& str) {
    if (str.size() == 0) {
        return std::string();
    }
    for (uint i = 0; i < str.size(); i++) {
        if (48 <= str[i] && str[i] <= 57) {
            continue;
        }
        std::string ret(str.begin(), str.begin() + i);
        str.erase(str.begin(), str.begin() + i + 1);
        return ret;
    }
    if (!str.empty()) {
        std::string ret(str);
        str.clear();
        return ret;
    }
    return str;
}

long LogMsg::Process(std::string& date) {
    std::tm t   = {};
    t.tm_year   = std::stoi(split_date(date)) - 1900;
    t.tm_mon    = std::stoi(split_date(date)) - 1;
    t.tm_mday   = std::stoi(split_date(date));
    t.tm_hour   = std::stoi(split_date(date));
    t.tm_min    = std::stoi(split_date(date));
    t.tm_sec    = std::stoi(split_date(date));
    int tm_msec = std::stoi(split_date(date));
    return mktime(&t) * 1000 + tm_msec;
    //%Y-%m-%d %H:%M:%S.%ms
    // return 0;
}

LogMsg::LogMsg() :
    formatted(false),
    thread_id(0),
    date(0),
    level(std::string()),
    detail(std::string()),
    source_location(std::string()),
    formatted_msg(std::string()) {}

LogMsg::LogMsg(const LogMsg& other) {
    operator=(other);
}

LogMsg::LogMsg(std::string log) :
    formatted(false),
    thread_id(0),
    source_location(std::string()),
    formatted_msg(std::string()) {
    std::string tmp_date = Process(log, ']'); // get date
    date                 = Process(tmp_date);
    level                = Process(log, ']'); // get level
    detail               = log;               // remaning detail
    if (detail[detail.size() - 1] == '\n') {
        detail.erase(detail.end() - 1);
    }
}

LogMsg::LogMsg(std::string date_, std::string level_, std::string detail_,
               int thread_id_, std::string source_location_) :
    formatted(false),
    thread_id(thread_id_),
    level(level_),
    detail(detail_),
    source_location(source_location_),
    formatted_msg(std::string()) {
    date = Process(date_);
}

LogMsg::LogMsg(long timestamp_ms, LogLevel level_, std::string detail_,
               int thread_id_, std::string source_location_) :
    formatted(false),
    thread_id(thread_id_),
    date(timestamp_ms),
    detail(detail_),
    source_location(source_location_),
    formatted_msg(std::string()) {
    level = Level_to_string(level_);
}

LogMsg::LogMsg(long timestamp_ms, LogLevel level_, const char* detail_,
               int thread_id_, const char* source_location_) :
    formatted(false),
    thread_id(thread_id_),
    date(timestamp_ms),
    detail(detail_),
    source_location(source_location_),
    formatted_msg(std::string()) {
    level = Level_to_string(level_);
}

void LogMsg::Format(std::string formatted_msg_) {
    formatted_msg = formatted_msg_;
    formatted     = true;
}

int& LogMsg::Get_thread_id() { return thread_id; }

long& LogMsg::Get_date() { return date; }

std::string& LogMsg::Get_level() { return level; }

std::string& LogMsg::Get_detail() { return detail; }

std::string& LogMsg::Get_source_location() { return source_location; }

std::string LogMsg::Output() const {
    if (formatted) { return formatted_msg; }
    return std::string();
}

LogMsg& LogMsg::operator=(const LogMsg& other) {
    this->formatted       = other.formatted;
    this->thread_id       = other.thread_id;
    this->date            = other.date;
    this->level           = other.level;
    this->detail          = other.detail;
    this->source_location = other.source_location;
    this->formatted_msg   = other.formatted_msg;
    return *this;
}

LogMsg::~LogMsg() {}