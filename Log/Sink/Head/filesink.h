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
    LogFormatter formatter;
    std::atomic<uint> count;
    std::atomic<uint> suc_count;
    std::string logs;
    FileHandler filehandler;
    void Fileout();
    void Init_open(std::string filename);

public:
    FileSink(std::string filename);
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