#include "Head/formatter.h"

extern const char* Loglevel_map[];

Formatter::Formatter(bool flag_) :flag(flag_) {
    log_tmp.reserve(1000);
    level_tmp.reserve(80);
    date_tmp.reserve(100);
    from_tmp.reserve(200);
    info_tmp.reserve(600);
}

void Formatter::loglevel(LogLevel level) {
    level_tmp = Loglevel_map[level % LEVELEND];
}

void Formatter::logdate(long date) {
    date_tmp.clear();
    memset(date_tmp_c, 0, 80);
    struct tm* time_tm = localtime(&date);
    strftime(date_tmp_c, 80, "[%T]", time_tm);
    date_tmp = date_tmp_c;
}

std::string Formatter::Trans(LogLevel level, long date, std::string from, std::string info) {
    if (flag) { mtx.lock(); }
    log_tmp.clear();
    logdate(date);
    loglevel(level);
    log_tmp += level_tmp;
    log_tmp += date_tmp;
    log_tmp += from;
    log_tmp += " : ";
    log_tmp += info;
    log_tmp += "\n";
    mtx.unlock();
    return log_tmp;
}

void Formatter::Trans(LogLevel level, long date, std::string from, std::string info, std::string& log) {
    if (flag) { mtx.lock(); }
    log.clear();
    logdate(date);
    loglevel(level);
    log += level_tmp;
    log += date_tmp;
    log += from;
    log += ":";
    log += info;
    log_tmp += "\n";
    mtx.unlock();
}