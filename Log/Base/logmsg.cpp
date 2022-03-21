#include "Head/logmsg.h"

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

LogMsg::LogMsg(std::string log) {
    level  = process(log, ']'); // get level
    date   = process(log, ']'); // get date
    info   = process(log, ':'); // get info
    detail = log;               // remaning detail
}

LogMsg::LogMsg(std::string level_, std::string date_,
               std::string info_, std::string detail_) :
    level(level_),
    date(date_),
    info(info_),
    detail(detail_) {}

std::string& LogMsg::Change_level() { return level; }

std::string& LogMsg::Change_date() { return date; }

std::string& LogMsg::Change_info() { return info; }

std::string& LogMsg::Change_detail() { return detail; }

LogMsg::~LogMsg() {}