#ifndef STD_SINK_H_
#define STD_SINK_H_

#include "../../../Class/noncopyable.h"
#include "logsink.h"
#include <fstream>

namespace Wasi {
namespace Log {

class StdSink : public LogSink,
                Noncopyable {
private:
    LogFormatter formatter;
    std::atomic<uint> count;
    std::mutex mtx;
    std::filebuf file;
    std::ostream os;
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