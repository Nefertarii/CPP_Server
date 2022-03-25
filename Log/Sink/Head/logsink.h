#ifndef LOG_SINK_H_
#define LOG_SINK_H_

#include <mutex>
#include <string>

namespace Wasi {
namespace Log {

class LogMsg;

class LogSink {
public:
    LogSink()                           = default;
    virtual void Logger(LogMsg& logmsg) = 0;
    virtual void Flush()                = 0;
    virtual ~LogSink()                  = default;
};

}
} // namespace Wasi::Log

#endif