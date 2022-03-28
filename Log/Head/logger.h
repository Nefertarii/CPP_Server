#ifndef LOG_LOGGER_H_
#define LOG_LOGGER_H_

#include "../Sink/Head/filesink.h"
#include "../Sink/Head/logsink.h"
#include <initializer_list>
#include <memory>

namespace Wasi {
namespace Log {

using ErrorHandler = std::function<void()>;
using SinkPtr      = std::shared_ptr<LogSink>;
using SinkInitList = std::initializer_list<SinkPtr>;
using SinkIt       = std::vector<SinkPtr>::iterator;

class Logger {
private:
    std::string name;
    std::function<void()> error_handler;
    std::vector<SinkPtr> sinks;
    LogLevel flush_level;
    void Log(LogMsg& logmsg);

public:
    Logger(std::string name_);
    Logger(std::string name_, SinkPtr sink_);
    Logger(std::string name_, SinkInitList sinks_);
    Logger(std::string name_, SinkIt beg, SinkIt end);
    void Debug(std::string detail, int tid = 0, std::string source = "");
    void Info(std::string detail, int tid = 0, std::string source = "");
    void Warning(std::string detail, int tid = 0, std::string source = "");
    void Error(std::string detail, int tid = 0, std::string source = "");
    void Critical(std::string detail, int tid = 0, std::string source = "");
    void Fatal(std::string detail, int tid = 0, std::string source = "");
    void Flush();
    void Flush_on(LogLevel level);
    void Set_formatter(LogFormat format);
    void Set_error_handler(ErrorHandler func);
    ~Logger();
};

}
} // namespace Wasi::Log

#endif