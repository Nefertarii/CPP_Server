#include "Head/sink.h"

const char* Loglevel_map[] = {
    [NORMAL] = "[   Normal   ]",
    [NOTIFI] = "[Notification]",
    [WARNING] = "[   Warning  ]",
    [ERROR] = "[    Error   ]",
    [CRITICAL] = "[  Critical  ]"
};

std::string Sink::Log_consume() {
    std::string tmp;
    tmp += level.front();
    tmp += " ";
    tmp += date.front();
    tmp += " ";
    tmp += from.front();
    tmp += " ";
    tmp += info.front();
    tmp += " ";
    log_size -= 1;
    level.pop();
    date.pop();
    from.pop();
    info.pop();
    return tmp;
}

std::string Sink::process(std::string& str) {
    uint size = str.size();
    if (size == 0) { std::string(); }
    uint index = 0;
    while (1) {
        if (str[index] == ' ') {
            std::string tmp(str.begin(), str.begin() + index);
            str.erase(str.begin(), str.begin() + index + 1);
            return tmp;
        }
        if (index + 1 == size) {
            std::string tmp(str);
            str.clear();
            return tmp;
        }
        index += 1;
    }
    return std::string();
}

Sink::Sink() {
    temp_capacity = 200;
    log_size = 0;
    filter = NONE;
    filename = "Default.log";
}

Sink::Sink(int capacity, LogLevel filter_, std::string save_file) {
    temp_capacity = capacity;
    log_size = 0;
    filter = filter_;
    filename = save_file;
}

void Sink::Set_capacity(uint size) {
    if (size < temp_capacity) {
        Log_add(WARNING, clock.Now_time_sec(),
                "Log sink",
                "Size small than origin size.");
        return;
    }
    temp_capacity = size;
}

void Sink::Set_filter(LogLevel level) { filter = level; }

void Sink::Set_filename(std::string save_file) { filename = save_file; }

void Sink::Log_add(std::string log) {
    if (flag) { mtx.lock(); }
    level.push(process(log));
    date.push(process(log));
    from.push(process(log));
    info.push(process(log));
    log_size += 1;
    mtx.unlock();
}

void Sink::Log_add(LogLevel log_level, long log_date,
                   std::string log_from, std::string log_info) {
    if (flag) { mtx.lock(); }
    level.push(Loglevel_map[log_level % LEVELEND]);
    date.push(std::to_string(log_date));
    from.push(log_from);
    info.push(log_info);
    log_size += 1;
    mtx.unlock();
}

void Sink::Log_consume(std::string& log) {
    if (log_size == 0) {
        Log_add(WARNING, clock.Now_time_sec(),
                "Log sink",
                "The log sink is empty.");
        return;
    }
    log = Log_consume();
}

void Sink::Log_consume(std::vector<std::string>* logs) {
    while (log_size != 0) {
        logs->push_back(Log_consume());
    }
}