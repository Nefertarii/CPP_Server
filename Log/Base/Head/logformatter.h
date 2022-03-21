#ifndef LOG_FORMATTER_H_
#define LOG_FORMATTER_H_

#include "loglevel.h"
#include "logmsg.h"
#include <Timer/Head/clock.h>

namespace Wasi {
namespace Log {

class LogFormatter {
private:
    std::string str_log;

public:
    LogFormatter();
    std::string Format(LogMsg logmsg);
    LogLevel Format_level(std::string level);
    ~LogFormatter();
};

}
} // namespace Wasi::Log

#endif