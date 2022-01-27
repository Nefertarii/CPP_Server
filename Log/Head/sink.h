#ifndef LOG_SINK_H_
#define LOG_SINK_H_

#include <string>
#include <queue>
#include <mutex>
#include "logenum.h"
#include "../../Headfile/Clock/Head/timer.h"

class Sink {
private:
    bool concurrency_flag;
    std::mutex mtx;
    std::queue<Log> log_queue;
    LogLevel in_filter;
    LogLevel out_filter;
    Timer clock;
    LogLevel Filter_str(std::string log_level);
public:
    Sink();
    Sink(bool concurrency_flag_);
    Sink(LogLevel in, LogLevel out, bool concurrency_flag_);
    void Set_flag(bool concurrency_flag_);
    void Set_in_filter(LogLevel level);
    void Set_out_filter(LogLevel level);
    bool Log_add(std::string log_str);
    bool Log_add(LogLevel log_level, long log_date,
                 std::string log_from, std::string log_detail);
    bool Log_consume(Log* log);
    bool Log_consume(std::vector<Log>* logs);
    std::string process(std::string& str, char stop);
};


#endif