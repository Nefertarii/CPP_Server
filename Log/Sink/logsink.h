#ifndef LOG_SINK_H_
#define LOG_SINK_H_

#include <Log/logformatter.h>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>

namespace Wasi {
namespace Log {

class LogMsg;

class LogSink {
protected:
    std::unique_ptr<LogFormatter> formatter;
    std::atomic<uint> count;
    std::string name;

public:
    virtual void Logger(LogMsg& msg)       = 0;
    virtual void Flush()                   = 0;
    virtual void Set_format(LogFormat fmt) = 0;
    virtual uint Get_count()               = 0;
    virtual ~LogSink()                     = default;
};

}
} // namespace Wasi::Log

#endif