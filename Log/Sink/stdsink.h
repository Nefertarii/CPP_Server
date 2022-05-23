#ifndef STD_SINK_H_
#define STD_SINK_H_

#include "logsink.h"
#include <Base/noncopyable.h>
#include <Log/logformatter.h>
#include <fstream>

namespace Wasi {
namespace Log {

class StdSink : public LogSink,
                Noncopyable {
private:
    LogFormatter formatter;
    std::mutex mtx;
    std::atomic<uint> count;
    void Stdout(std::string message);

public:
    StdSink();
    explicit StdSink(LogFormat logformat);
    void Logger(LogMsg& logmsg);
    void Flush();
    void Set_format(LogFormat fmt);
    uint Get_count();
    ~StdSink() override;
};

}
} // namespace Wasi::Log

#endif