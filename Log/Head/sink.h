#ifndef LOG_SINK_H_
#define LOG_SINK_H_

#include <string>
#include <queue>
#include <mutex>
#include "logenum.h"
#include "../../Headfile/Clock/Head/timer.h"

extern const char* Loglevel_map[];

class Sink {
private:
    bool flag;
    std::mutex mtx;
    std::queue<std::string> level;
    std::queue<std::string> date;
    std::queue<std::string> from;
    std::queue<std::string> info;
    uint temp_capacity;
    uint log_size;
    LogLevel filter;
    std::string filename;
    Timer clock;
    std::string Log_consume();
    std::string process(std::string& str);
public:
    Sink();
    Sink(int capacity, LogLevel filter_, std::string save_file);
    void Set_capacity(uint size);
    void Set_filter(LogLevel level);
    void Set_filename(std::string save_file);
    void Log_add(std::string log);
    void Log_add(LogLevel log_level, long log_date,
                 std::string log_from, std::string log_info);
    void Log_consume(std::string& log);
    void Log_consume(std::vector<std::string>* logs);
};




#endif