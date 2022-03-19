#ifndef BASE_LOGLEVEL_H_
#define BASE_LOGLEVEL_H_

#include <string>

namespace Wasi {
namespace Log {

enum class LogLevel : uint32_t {
    UNINITIALIZED = 0,          // uinit
    NONE          = 1,          // none
    DBG           = 1000,       // debug
    INFO          = 2000,       // info
    WARN          = 3000,       // warning,warn
    ERR           = 4000,       // err,error
    CRITICAL      = 5000,       // critical
    FATAL         = 0x7fffffff, // fatal
};

LogLevel String_to_Level(std::string name);
std::string Level_to_string(LogLevel level);
// std::ostream &operator<<(std::ostream &os, LogLevel level);

}
} // namespace Wasi::Log

#endif