#ifndef LOGGER_FORMATTER_H_
#define LOGGER_FORMATTER_H_

#include "Include/Logger/header.h"

namespace wasi {
namespace log {

std::string LevelToString(LogLevel level);
LogLevel StringToLevel(std::string level);

class LogFormatter {
private:
    std::string tmpstr_;
    LoggerSetting formater_;
    LogMsg tmplog_;
    void ProcessingDate();
    void ProcessingHead();
    void ProcessingMsg();
public:
    LogFormatter();
    void SetFormat(LoggerSetting setting);
    void Format(LogMsg& msg);
    void Format(LogMsg& msg, std::string& dest);
    ~LogFormatter();
};

}
} // namespace wasi::Log

#endif