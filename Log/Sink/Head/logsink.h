#ifndef LOG_SINK_H_
#define LOG_SINK_H_

#include "Log/Base/Head/logmsg.h"
#include <mutex>
#include <string>

namespace Wasi {
namespace Log {

class LogSink {
public:
    virtual LogSink()                  = default;
    virtual void Logger(LogMsg logmsg) = 0;
    virtual void Flush()               = 0;
    virtual ~LogSink()                 = 0;
};

}
} // namespace Wasi::Log

#endif