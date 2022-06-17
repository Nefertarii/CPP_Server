#ifndef LOG_LOGGER_H_
#define LOG_LOGGER_H_

#include "Sink/logsink.h"
#include <functional>
#include <initializer_list>
#include <memory>
#include <vector>

namespace Wasi {
namespace Log {
using ErrorHandler = std::function<void()>;
using SinkPtr      = std::shared_ptr<LogSink>;
using SinkInitList = std::initializer_list<SinkPtr>;
using SinkIt       = std::vector<SinkPtr>::iterator;

class Logger {
private:
    std::mutex mtx;
    std::string name;
    std::function<void()> error_handler;
    std::vector<SinkPtr> sinks;
    LogLevel flush_level;
    void Log(LogMsg& logmsg);
    void Splice(std::string& file, std::string func, int line);

public:
    Logger(std::string name_);
    Logger(std::string name_, SinkPtr sink_);
    Logger(std::string name_, SinkInitList sinks_);
    Logger(std::string name_, SinkIt beg, SinkIt end);
    void Debug(std::string detail, int tid = 0, std::string file = "",
               std::string func = "", int line = 0);
    void Info(std::string detail, int tid = 0, std::string file = "",
              std::string func = "", int line = 0);
    void Warning(std::string detail, int tid = 0, std::string file = "",
                 std::string func = "", int line = 0);
    void Error(std::string detail, int tid = 0, std::string file = "",
               std::string func = "", int line = 0);
    void Critical(std::string detail, int tid = 0, std::string file = "",
                  std::string func = "", int line = 0);
    void Fatal(std::string detail, int tid = 0, std::string file = "",
               std::string func = "", int line = 0);
    size_t Size();
    void Flush();
    void Flush_on(LogLevel level);
    void Push_back(SinkPtr sink_);
    void Remove(uint i);
    void Set_formatter(LogFormat format);
    void Set_error_handler(ErrorHandler func);
    ~Logger();
};

}
} // namespace Wasi::Log

#endif