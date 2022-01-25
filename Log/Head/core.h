#ifndef LOG_CORE_H_
#define LOG_CORE_H_

#include "formatter.h"
#include "sink.h"
#include "record.h"

class Log_Core {
private:
    Timer clock;
    Formatter formatter;
    Sink sink;
    Record record;
    uint temp_capacity;
    uint log_size;
public:
    //
};


#endif