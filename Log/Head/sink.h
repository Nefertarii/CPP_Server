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
    std::queue<std::string> level;
    std::queue<std::string> date;
    std::queue<std::string> from;
    std::queue<std::string> info;
    LogLevel filter;
    Timer clock;
    uint log_size;
    std::string Log_consume();
    std::string process(std::string& str, char stop);
public:
    Sink();
    Sink(bool concurrency_flag_);
    Sink(LogLevel filter_, bool concurrency_flag_);
    void Set_flag(bool concurrency_flag_);
    void Set_filter(LogLevel level);
    void Log_add(std::string log);
    void Log_add(LogLevel log_level, long log_date,
                 std::string log_from, std::string log_detail);
    void Normal_log(std::string log_from, std::string log_detail);
    void Notifi_log(std::string log_from, std::string log_detail);
    void Warning_log(std::string log_from, std::string log_detail);
    void Error_log(std::string log_from, std::string log_detail);
    void Critical_log(std::string log_from, std::string log_detail);
    void Log_consume(std::string& log);
    void Log_consume(std::vector<std::string>* logs);
    uint Get_size();
};


#endif