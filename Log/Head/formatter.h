#ifndef LOG_FORMATTER_H_
#define LOG_FORMATTER_H_

#include <cstring>
#include <string>
#include <mutex>
#include "logenum.h"

class Formatter {
private:
    bool concurrency_flag;
    std::mutex mtx;
    char date_tmp_c[80];
public:
    //[level] [date] from:info  
    Formatter();
    Formatter(bool concurrency_flag_);
    void Set_flag(bool concurrency_flag_);
    std::string Trans_date(long date);
    std::string Trans_date(std::string date);
    std::string Trans_log(LogLevel level, long date, std::string from, std::string info);
    std::string Trans_log(Log log);
};


#endif