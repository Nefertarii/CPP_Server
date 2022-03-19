#include "Head/logline.h"

using namespace Wasi::Log;

std::string LogLine::process(std::string& str, char stop) {
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
        std::cout << "LogLine::process Logline input error\n";
        return std::string();
    }
    return std::string();
}

LogLine::LogLine(std::string log) {
    std::string tmp = process(log, ']');
    level           = String_to_Level(tmp);
    tmp             = process(log, ']');
    date            = std::stol(tmp);
    info            = process(log, ':');
    detail          = log;
}

LogLine::LogLine(LogLevel level_, long date_,
                 std::string info_, std::string detail_) :
    level(level_),
    data(date_),
    info(info_),
    detail(detail_) {}

LogLevel& LogLine::Change_level() { return level; }

long& LogLine::Change_date() { return date; }

std::string& LogLine::Change_info() { return info; }

std::string& LogLine::Change_detail() { return detail; }

LogLine::~LogLine() {}