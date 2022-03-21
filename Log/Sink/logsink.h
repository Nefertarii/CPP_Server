#ifndef LOG_SINK_H_
#define LOG_SINK_H_

#include "Log/Base/Head/logmsg.h"
#include <mutex>
#include <string>

namespace Wasi {
namespace Log {

class LogSink {
public:
    virtual Log(LogMsg logmsg) = 0;
    virtual Flush()            = 0;
    virtual ~LogSink()         = default;
};

}
} // namespace Wasi::Log

#endif