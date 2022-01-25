#include "Head/formatter.h"

extern const char* Loglevel_map[];

Formatter::Formatter() {
    concurrency_flag = false;
    log_tmp.reserve(1000);
}

Formatter::Formatter(bool concurrency_flag_) {
    concurrency_flag = concurrency_flag_;
    log_tmp.reserve(1000);
}

std::string Formatter::Trans_date(long date) {
    date_tmp.clear();
    memset(date_tmp_c, 0, 80);
    struct tm* time_tm = localtime(&date);
    strftime(date_tmp_c, 80, "[%T]", time_tm);
    date_tmp = date_tmp_c;
    return date_tmp;
}

std::string Formatter::Trans_date(std::string date) {
    return Trans_date(std::stol(date));
}

std::string Formatter::Trans_log(LogLevel level, long date, std::string from, std::string info) {
    if (concurrency_flag) { mtx.lock(); }
    log_tmp.clear();
    log_tmp += Loglevel_map[level];
    log_tmp += Trans_date(date);
    log_tmp += from;
    log_tmp += ":";
    log_tmp += info;
    log_tmp += "\n";
    mtx.unlock();
    return log_tmp;
}

void Formatter::Trans_log(LogLevel level, long date, std::string from, std::string info, std::string& log) {
    if (concurrency_flag) { mtx.lock(); }
    log.clear();
    log += Loglevel_map[level];
    log += Trans_date(date);
    log += from;
    log += ":";
    log += info;
    log += "\n";
    mtx.unlock();
}