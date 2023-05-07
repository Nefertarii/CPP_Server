#ifndef LOGGER_APPENDER_H_
#define LOGGER_APPENDER_H_

#include "Include/Logger/formatter.h"
#include "Include/Logger/Sink/logsink.h"
#include <vector>
#include <mutex>
#include <functional>
#include <initializer_list>
#include <memory>

namespace wasi {
namespace log {
using SinkPtr      = std::shared_ptr<LogSink>;
using SinkInitList = std::initializer_list<SinkPtr>;
using SinkIt       = std::vector<SinkPtr>::iterator;
using ErrorHandler = std::function<void()>;

class Appender {
private:
    std::mutex mtx_;
    std::string name_;
    std::function<void()> error_handler_;
    std::vector<SinkPtr> sinks_;
    LogFormatter formater_;
    LogLevel filter_levels_;
    //return true mean filter
    bool Filter(LogLevel level);
    void Splice(std::string& file, std::string func, int line);
    void Log(LogMsg& logmsg);

public:
    Appender(std::string name);
    Appender(std::string name, SinkPtr sink);
    Appender(std::string name, SinkInitList sinks);
    Appender(std::string name, SinkIt beg, SinkIt end);
    bool Log(LogLevel level, std::string content, int tid, std::string file,
             std::string func, int line);
    size_t Size();
    void Flush();
    // push sink
    void PushBack(SinkPtr sink_);
    bool Remove(size_t i);
    std::vector<std::string> GetListName();
    void SetFilterLevel(LogLevel filter);
    void CancelFilterLevel(LogLevel filter);
    void SetFormatter(LogFormatter formater);
    void SetErrorHandler(ErrorHandler func);
    ~Appender();
};

}
} // namespace wasi::log

#endif