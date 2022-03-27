#ifndef LOG_LOGGER_H_
#define LOG_LOGGER_H_

#include "../../Sink/Head/filesink.h"
#include "../../Sink/Head/logsink.h"
#include <memory>

namespace Wasi {
namespace Log {

using ErrorHandler = std::function<void()>;
using SinkPtr      = std::shared_ptr<LogSink>;

class Logger {
private:
    std::string name;
    std::function<void()> error_handler;
    std::vector<SinkPtr> sinks;
    template <class... Args>
    void Log(LogLevel level, Args... args);

public:
    Logger(std::string name_);
    template <class... Args>
    void Debug(Args... args);
    template <class... Args>
    void Info(Args... args);
    template <class... Args>
    void Warning(Args... args);
    template <class... Args>
    void Error(Args... args);
    template <class... Args>
    void Critical(Args... args);
    template <class... Args>
    void Fatal(Args... args);
    void Flush();
    void Flush_on(LogLevel level);
    void Set_formatter(LogFormat format);
    void Set_error_handler(ErrorHandler func);
    ~Logger();
};

}
} // namespace Wasi::Log

#endif