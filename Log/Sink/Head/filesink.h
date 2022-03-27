#ifndef FILE_SINK_H_
#define FILE_SINK_H_

#include "../../../Class/noncopyable.h"
#include "../../Base/Head/filehandler.h"
#include "../../Base/Head/logformatter.h"
#include "logsink.h"
#include <atomic>
#include <mutex>

namespace Wasi {
namespace Log {

class FileSink : public LogSink,
                 Noncopyable {
private:
    std::unique_ptr<LogFormatter> formatter;
    std::atomic<uint> count;
    std::mutex mtx;
    std::string logs;
    FileHandler file;
    void Fileout(std::string message);

public:
    FileSink(LogFormat logformat, std::string filename);
    FileSink(LogFormat logformat, std::string filename, FileEvents events);
    void Logger(LogMsg& logmsg);
    void Flush();
    void Set_format(LogFormat logformat);
    uint Get_count();
    ~FileSink() override;
};

}
} // namespace Wasi::Log

#endif