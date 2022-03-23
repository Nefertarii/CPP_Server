#ifndef LOG_FORMATTER_H_
#define LOG_FORMATTER_H_

#include "loglevel.h"
#include "logmsg.h"
#include <map>

namespace Wasi {
namespace Log {

class LogFormatter {
public:
    struct LogFormat {
        bool print_thread_id;
        bool print_source_location;
        const char* data_format;
        std::map<LogLevel, std::string> consoles_color;
    };

private:
    std::string tmp_logmsg;
    LogFormat log_format;
    void process_date(long timestamp_ms, const char* format);

public:
    // [Thread:1000][FILE,FUNC,LINE][2022-3-23 21:00:00.000] print log
    LogFormatter();
    std::string Format(LogMsg& logmsg);
    void Format(LogMsg& logmsg, std::string& dest);
    void Set_format();
    void Set_consoles_color(LogLevel level, std::string color);
    ~LogFormatter();
};

}
} // namespace Wasi::Log

#endif