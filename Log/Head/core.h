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
    uint temp_capacity;
public:
    Core();
    void Normal_log(std::string log_from, std::string log_detail);
    void Notifi_log(std::string log_from, std::string log_detail);
    void Warning_log(std::string log_from, std::string log_detail);
    void Error_log(std::string log_from, std::string log_detail);
    void Critical_log(std::string log_from, std::string log_detail);
    
    ~Core();
};


#endif