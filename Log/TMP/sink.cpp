#include "Head/sink.h"
#include <iostream>

using namespace Wasi::Log;

const char* Loglevel_map[] = {
    [LOG_NORMAL]   = "   Normal   ",
    [LOG_NOTICE]   = "Notification",
    [LOG_WARNING]  = "   Warning  ",
    [LOG_ERROR]    = "   Error    ",
    [LOG_CRITICAL] = "   Critical "
};

LogLevel Sink::Filter_str(std::string log_level) {
    if (log_level == "Normal") { return LOG_NORMAL; }
    else if (log_level == "Notification") { return LOG_NOTICE; }
    else if (log_level == "Warning") { return LOG_WARNING; }
    else if (log_level == "Error") { return LOG_ERROR; }
    else if (log_level == "Critical") { return LOG_CRITICAL; }
    else { return LOG_END; }
}

/*std::string Sink::Log_consume() {
    if (log_queue.empty()) { return std::string(); }
    if (concurrency_flag) { mtx.lock(); }
    Log tmp_log = log_queue.front();
    log_queue.pop();
    if (out_filter != Filter_str(tmp_log.level)) {
        mtx.unlock();
        return std::string();
    }
    std::string tmp_str;
    tmp_str += tmp_log.level;
    tmp_str += " ";
    tmp_str += tmp_log.date;
    tmp_str += " ";
    tmp_str += tmp_log.from;
    tmp_str += ":";
    tmp_str += tmp_log.detail;
    mtx.unlock();
    return tmp_str;
}*/

Sink::Sink() {
    concurrency_flag = false;
    in_filter = LOG_NONE;
    out_filter = LOG_NONE;
}

Sink::Sink(LogLevel in, LogLevel out, bool concurrency_flag_) {
    concurrency_flag = concurrency_flag_;
    in_filter = in;
    out_filter = out;
}

void Sink::Set_flag(bool concurrency_flag_) { concurrency_flag = concurrency_flag_; }

void Sink::Set_in_filter(LogLevel level) { in_filter = level; }

void Sink::Set_out_filter(LogLevel level) { out_filter = level; }

bool Sink::Log_add(std::string log_str) {
    if (concurrency_flag == true) { mtx.lock(); }
    std::string log_level = process(log_str, ' ');
    if (in_filter >= Filter_str(log_level)) {
        mtx.unlock();
        return false;
    }
    LogLine tmp_log;
    tmp_log.level = log_level;
    tmp_log.date = process(log_str, ' ');
    tmp_log.from = process(log_str, ':');
    tmp_log.detail = log_str;
    log_queue.push(tmp_log);
#ifdef DEBUG
    std::cout << tmp_log << "\n";
#endif
    mtx.unlock();
    return true;
}

bool Sink::Log_add(LogLevel log_level, long log_date,
                   std::string log_from, std::string log_detail) {
    if (in_filter >= log_level) { return false; }
    if (concurrency_flag == true) { mtx.lock(); }
    LogLine tmp_log;
    tmp_log.level = Loglevel_map[log_level];
    tmp_log.date = std::to_string(log_date);
    tmp_log.from = log_from;
    tmp_log.detail = log_detail;
    log_queue.push(tmp_log);
#ifdef DEBUG
    std::cout << tmp_log << "\n";
#endif
    mtx.unlock();
    return true;
}

bool Sink::Log_consume(LogLine* log) {
    if (log_queue.empty()) { return false; }
    if (concurrency_flag) { mtx.lock(); }
    LogLine tmp_log = log_queue.front();
    log_queue.pop();
    if (out_filter == LOG_NONE) {
        *log = tmp_log;
        mtx.unlock();
        return true;
    }
    else if (out_filter != Filter_str(tmp_log.level)) {
        mtx.unlock();
        return false;
    }
    else {
        *log = tmp_log;
        mtx.unlock();
        return true;
    }


}

bool Sink::Log_consume(std::queue<LogLine>* logs) {
    if (log_queue.empty()) { return false; }
    if (concurrency_flag) { mtx.lock(); }
    LogLine tmp_log;
    while (!log_queue.empty()) {
        tmp_log = log_queue.front();
        log_queue.pop();
        if (out_filter == LOG_NONE) {
            logs->push(tmp_log);
            continue;
        }
        else if (out_filter != Filter_str(tmp_log.level)) { continue; }
        else { logs->push(tmp_log); }
    }
    mtx.unlock();
    return true;
}

std::string Sink::process(std::string& str, char stop) {
    size_t size = str.size();
    if (size == 0) { std::string(); }
    size_t index = 0;
    while (1) {
        if (str[index] == stop) {
            std::string tmp(str.begin(), str.begin() + index);
            str.erase(str.begin(), str.begin() + index + 1);
            return tmp;
        }
        index += 1;
    }
    return std::string();
}

