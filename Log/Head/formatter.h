#ifndef LOG_FORMATTER_H_
#define LOG_FORMATTER_H_

#include <cstring>
#include <string>
#include <mutex>
#include "logenum.h"



class Formatter {
private:
    bool flag;
    std::mutex mtx;
    char date_tmp_c[80];
    std::string log_tmp;
    std::string level_tmp, date_tmp, from_tmp, info_tmp;
    void loglevel(LogLevel level);
    void logdate(long date);
public:
    //[level] [date] from : info  
    Formatter(bool flag_);
    std::string Trans(LogLevel level, long date, std::string from, std::string info);
    void Trans(LogLevel level, long date, std::string from, std::string info, std::string& log);
};




#endif