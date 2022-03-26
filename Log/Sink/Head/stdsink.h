#ifndef STD_SINK_H_
#define STD_SINK_H_

#include "../../../Class/noncopyable.h"
#include "../../Base/Head/logformatter.h"
#include "logsink.h"
#include <atomic>
#include <mutex>

namespace Wasi {
namespace Log {

class StdSink : public LogSink,
                Noncopyable {
private:
    LogFormatter formatter;
    LogMsg logline;
    std::atomic<uint> logcount;
    LogLevel filter_in;
    LogLevel filter_out;
    std::mutex mtx;
    void Stdout();

public:
    StdSink(LogFormat logformat);
    void Logger(LogMsg logmsg);
    void Flush();
    void Set_format(LogFormat logformat);
    void Set_filter_in(LogLevel level);
    void Set_filter_out(LogLevel level);
    uint Get_count();
    ~StdSink();
};

}
} // namespace Wasi::Log

#endif