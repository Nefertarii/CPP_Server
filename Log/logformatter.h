#ifndef LOG_FORMATTER_H_
#define LOG_FORMATTER_H_

#include "logmsg.h"
#include <map>

namespace Wasi {
namespace Log {

struct LogFormat {
    bool print_color;
    bool print_thread_id;
    bool print_source_location;
    const char* data_format;
    std::map<LogLevel, std::string> consoles_color;
    LogFormat();
};

class LogFormatter {
private:
    std::string tmp_logmsg;
    LogFormat log_format;
    std::string process_date(long timestamp_ms, const char* format);

public:
    // [Thread:1000][FILE,FUNC,LINE][2022-3-23 21:00:00.000][LEVEL] print log
    LogFormatter();
    std::string Format(LogMsg& logmsg);
    void Format(LogMsg& logmsg, std::string& dest);
    void Set_format(LogFormat log_format_);
    ~LogFormatter();
};

}
} // namespace Wasi::Log

#endif