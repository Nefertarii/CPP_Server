#ifndef LOG_CORE_H_
#define LOG_CORE_H_

#include "formatter.h"
#include "sink.h"
#include "record.h"

class Core {
private:
    Timer clock;
    Formatter formatter;
    Sink sink;
    Record record;
    uint log_size;
    uint log_capacity;
    std::queue<std::string> logs;
    std::queue<Log> logs_tmp;
public:
    Core();
    Core(bool flag, uint capacity, LogLevel in_filter, LogLevel out_filter, std::string filename);
    bool Normal_log(std::string log_from, std::string log_detail);
    bool Notifi_log(std::string log_from, std::string log_detail);
    bool Warning_log(std::string log_from, std::string log_detail);
    bool Error_log(std::string log_from, std::string log_detail);
    bool Critical_log(std::string log_from, std::string log_detail);
    void Set_log_in_filter(LogLevel level);
    void Set_log_out_filter(LogLevel level);
    void Set_save_file(std::string filename);
    void Set_concurrency(bool flag);
    void Set_capacity(uint size);
    void Save_to_file();
    ~Core() {}
};


#endif