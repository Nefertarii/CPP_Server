#include "Head/formatter.h"

extern const char* Loglevel_map[];

Formatter::Formatter() {
    concurrency_flag = false;
}

Formatter::Formatter(bool concurrency_flag_) {
    concurrency_flag = concurrency_flag_;
}

void Formatter::Set_flag(bool concurrency_flag_) {
    concurrency_flag = concurrency_flag_;
}

std::string Formatter::Trans_date(long date) {
    std::string date_tmp;
    memset(date_tmp_c, 0, 80);
    struct tm* time_tm = localtime(&date);
    strftime(date_tmp_c, 80, "%T", time_tm);
    date_tmp = date_tmp_c;
    return date_tmp;
}

std::string Formatter::Trans_date(std::string date) {
    return Trans_date(std::stol(date));
}

std::string Formatter::Trans_log(LogLevel level, long date, std::string from, std::string detail) {
    std::string tmp_log;
    if (concurrency_flag) { mtx.lock(); }
    tmp_log += '[';
    tmp_log += Loglevel_map[level];
    tmp_log += ']';
    tmp_log += Trans_date(date);
    tmp_log += from;
    tmp_log += ":";
    tmp_log += detail;
    tmp_log += "\n";
    mtx.unlock();
    return tmp_log;
}

std::string Formatter::Trans_log(Log log) {
    std::string tmp_log;
    if (concurrency_flag) { mtx.lock(); }
    tmp_log += '[';
    tmp_log += log.level;
    tmp_log += ']';
    tmp_log += '[';
    tmp_log += Trans_date(log.date);
    tmp_log += ']';
    tmp_log += log.from;
    tmp_log += ":";
    tmp_log += log.detail;
    tmp_log += "\n";
    mtx.unlock();
    return tmp_log;
}