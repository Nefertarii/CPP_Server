#include "Head/logmsg.h"

using namespace Wasi;
using namespace Wasi::Log;

std::string LogMsg::process(std::string& str, char stop) {
    size_t size = str.size();
    if (size == 0) { return std::string(); }
    size_t index = 0;
    while (1) {
        index = str.find_first_of(stop);
        if (index >= size) {
            std::string tmp(str.begin() + 1, str.begin() + index);
            str.erase(str.begin(), str.begin() + index + 1);
            return tmp;
        }
        std::cout << "LogMsg::process LogMsg input error\n";
        return std::string();
    }
    return std::string();
}

LogMsg::LogMsg(std::string log) :
    formatted(false),
    thread_id(0),
    source_location(std::string()),
    formatted_msg(std::string()) {
    date   = process(log, ']'); // get date
    level  = process(log, ']'); // get level
    detail = log;               // remaning detail
}

LogMsg::LogMsg(int thread_id_, std::string date,
               std::string level_, std::string detail_,
               std::string source_location_) :
    formatted(false),
    thread_id(thread_id_),
    level(level_),
    detail(detail_),
    source_location(source_location_),
    formatted_msg(std::string()) {
    date = std::stol(date_);
}

LogMsg::LogMsg(int thread_id_, long timestamp_ms,
               LogLevel level_, const char* detail_,
               const char* source_location_) :
    formatted(false),
    date(timestamp_ms),
    thread_id(thread_id_),
    detail(detail_),
    source_location(source_location_),
    formatted_msg(std::string()) {
    level = Level_to_string(level_);
}

void LogMsg::Format(std::string formatted_msg_) {
    formatted_msg = formatted_msg_;
    formatted     = true;
}

std::string& LogMsg::Get_thread_id() { return thread_id; }

std::string& LogMsg::Get_level() { return level; }

std::string& LogMsg::Get_date() { return date; }

std::string& LogMsg::Get_detail() { return detail; }

std::string& LogMsg::Get_source_location() { return source_location; }

std::string LogMsg::Output() const {
    if (formatted) { return formatted_msg; }
    return std::string();
}

LogMsg::~LogMsg() {}