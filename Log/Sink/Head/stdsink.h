#ifndef STD_SINK_H_
#define STD_SINK_H_

#include "../../../Class/noncopyable.h"
#include "../../Base/Head/filehandler.h"
#include "../../Base/Head/logconfig.h"
#include "../../Base/Head/logformatter.h"
#include "logsink.h"
#include <atomic>
#include <mutex>

namespace Wasi {
namespace Log {

class StdSink : public LogSink,
                Noncopyable {
private:
    LogConfig* sink_settings;
    LogFormatter formatter;
    std::string logline;
    std::atomic<uint> logcount;
    std::mutex mtx;
    void Stdout();

public:
    StdSink(LogConfig* sink_settings_);
    void Logger(const LogMsg& logmsg);
    void Flush();
    ~StdSink();
};

}
} // namespace Wasi::Log

#endif