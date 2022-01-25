#include "Head/sink.h"

const char* Loglevel_map[] = {
    [LOG_NORMAL]   = "[   Normal   ]",
    [LOG_NOTIFI]   = "[Notification]",
    [LOG_WARNING]  = "[   Warning  ]",
    [LOG_ERROR]    = "[    Error   ]",
    [LOG_CRITICAL] = "[  Critical  ]"
};

std::string Sink::Log_consume() {
    if (log_size == 0) {
        return std::string();
    }
    if (concurrency_flag) { mtx.lock(); }
    std::string tmp;
    tmp += level.front();
    tmp += " ";
    tmp += date.front();
    tmp += " ";
    tmp += from.front();
    tmp += ":";
    tmp += info.front();
    log_size -= 1;
    level.pop();
    date.pop();
    from.pop();
    info.pop();
    mtx.unlock();
    return tmp;
}

std::string Sink::process(std::string& str, char stop) {
    uint size = str.size();
    if (size == 0) { std::string(); }
    uint index = 0;
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

Sink::Sink() {
    concurrency_flag = false;
    log_size = 0;
    filter = LOG_NONE;
}

Sink::Sink(bool concurrency_flag_) {
    concurrency_flag = concurrency_flag_;
    log_size = 0;
    filter = LOG_NONE;
}

Sink::Sink(LogLevel filter_, bool concurrency_flag_) {
    concurrency_flag = concurrency_flag_;
    log_size = 0;
    filter = filter_;
}

void Sink::Set_flag(bool concurrency_flag_) {
    concurrency_flag = concurrency_flag_;
}

void Sink::Set_filter(LogLevel level) { filter = level; }

void Sink::Log_add(std::string log) {
    if (concurrency_flag) { mtx.lock(); }
    level.push(process(log, ' '));
    std::string tmp = process(log, ' ');
    date.push(process(log, ' '));
    from.push(process(log, ':'));
    info.push(log);
    log_size += 1;
    mtx.unlock();
}

void Sink::Log_add(LogLevel log_level, long log_date,
                   std::string log_from, std::string log_detail) {
    if (concurrency_flag) { mtx.lock(); }
    level.push(Loglevel_map[log_level]);
    date.push(std::to_string(log_date));
    from.push(log_from);
    info.push(log_detail);
    log_size += 1;
    mtx.unlock();
}

void Sink::Normal_log(std::string log_from, std::string log_detail) {
    Log_add(LOG_NORMAL, clock.Now_time_sec(), log_from, log_detail);
}

void Sink::Notifi_log(std::string log_from, std::string log_detail) {
    Log_add(LOG_NOTIFI, clock.Now_time_sec(), log_from, log_detail);
}

void Sink::Warning_log(std::string log_from, std::string log_detail) {
    Log_add(LOG_WARNING, clock.Now_time_sec(), log_from, log_detail);
}

void Sink::Error_log(std::string log_from, std::string log_detail) {
    Log_add(LOG_ERROR, clock.Now_time_sec(), log_from, log_detail);
}

void Sink::Critical_log(std::string log_from, std::string log_detail) {
    Log_add(LOG_CRITICAL, clock.Now_time_sec(), log_from, log_detail);
}

void Sink::Log_consume(std::string& log) {
    log = Log_consume();
}

void Sink::Log_consume(std::vector<std::string>* logs) {
    while (log_size != 0) {
        logs->push_back(Log_consume());
    }
}

uint Sink::Get_size() { return log_size; }