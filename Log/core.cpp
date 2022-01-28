#include "Head/core.h"

Core::Core() {
    log_size = 0;
    log_capacity = 200;
    Set_concurrency(false);
    sink.Set_in_filter(LOG_NONE);
    sink.Set_out_filter(LOG_NONE);
    record.Set_filename("Default_log.txt");
    
}

Core::Core(bool flag, uint capacity, LogLevel in_filter,
           LogLevel out_filter, std::string filename) {
    log_size = 0;
    log_capacity = capacity;
    Set_concurrency(flag);
    sink.Set_in_filter(in_filter);
    sink.Set_out_filter(out_filter);
    record.Set_filename(filename);
}

bool Core::Normal_log(std::string log_from, std::string log_detail) {
    if (log_size + 1 >= log_capacity) { return false; }
    if (sink.Log_add(LOG_NORMAL, clock.Now_time_sec(),
                     log_from, log_detail) == true) {
        log_size += 1;
        return true;
    }
    return false;
}

bool Core::Notifi_log(std::string log_from, std::string log_detail) {
    if (log_size + 1 >= log_capacity) { return false; }
    if (sink.Log_add(LOG_NOTIFI, clock.Now_time_sec(),
                     log_from, log_detail) == true) {
        log_size += 1;
        return true;
    }
    return false;
}

bool Core::Warning_log(std::string log_from, std::string log_detail) {
    if (log_size + 1 >= log_capacity) { return false; }
    if (sink.Log_add(LOG_WARNING, clock.Now_time_sec(),
                     log_from, log_detail) == true) {
        log_size += 1;
        return true;
    }
    return false;
}

bool Core::Error_log(std::string log_from, std::string log_detail) {
    if (log_size + 1 >= log_capacity) { return false; }
    if (sink.Log_add(LOG_ERROR, clock.Now_time_sec(),
                     log_from, log_detail) == true) {
        log_size += 1;
        return true;
    }
    return false;
}

bool Core::Critical_log(std::string log_from, std::string log_detail) {
    if (log_size + 1 >= log_capacity) { return false; }
    if (sink.Log_add(LOG_CRITICAL, clock.Now_time_sec(),
                     log_from, log_detail) == true) {
        log_size += 1;
        return true;
    }
    return false;
}

void Core::Set_log_in_filter(LogLevel level) { sink.Set_in_filter(level); }

void Core::Set_log_out_filter(LogLevel level) { sink.Set_out_filter(level); }

void Core::Set_save_file(std::string filename) { record.Set_filename(filename); }

void Core::Set_concurrency(bool flag) {
    formatter.Set_flag(flag);
    sink.Set_flag(flag);
    record.Set_flag(flag);
}

void Core::Set_capacity(uint size) { log_capacity = size; }

void Core::Save_to_file() {
    //consume
    std::string write_time = clock.Sec_to_string(clock.Now_time_sec());
    record.Save_to_file(write_time);
    sink.Log_consume(&logs_tmp);
    for (uint i = 0;i < logs_tmp.size();i++) {
        logs.push(formatter.Trans_log(logs_tmp.front()));
    }
    record.Save_to_file(&logs);
}