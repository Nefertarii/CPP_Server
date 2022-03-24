#ifndef STD_SINK_H_
#define STD_SINK_H_

#include "../../../Class/noncopyable.h"
#include "../../Base/Head/filehandler.h"
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
    std::string logline;
    std::atomic<uint> logcount;
    LogLevel filter_in;
    LogLevel fileter_out;
    std::mutex mtx;
    void Stdout();

public:
    StdSink(LogFormat logformat);
    void Logger(LogMsg& logmsg);
    void Flush();
    ~StdSink();
};

}
} // namespace Wasi::Log

#endif