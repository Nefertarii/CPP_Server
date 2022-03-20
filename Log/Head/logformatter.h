#ifndef LOG_FORMATTER_H_
#define LOG_FORMATTER_H_

#include "Base/Head/loglevel.h"
#include "Base/Head/logline.h"
#include <Timer/Head/clock.h>

namespace Wasi {
namespace Log {

class LogFormatter {
private:
    std::string str_log;

public:
    LogFormatter();
    std::string Format(LogLine logline);
    LogLevel Format_level(std::string level);
    
    ~LogFormatter();
};

}
} // namespace Wasi::Log

#endif