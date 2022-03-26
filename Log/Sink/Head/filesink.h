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
    LogMsg logline;
    FileHandler filehandler;
    LogLevel filter_in;
    LogLevel filter_out;
    uint max_buffer;
    std::string log_filename;
    std::mutex mtx;
    std::vector<LogMsg> logvec;
    std::atomic<uint> logcount;
    void Fileout();
    void Save_to_file();

public:
    FileSink(LogFormat logformat);
    void Logger(LogMsg logmsg);
    void Flush();
    void Set_format(LogFormat logformat);
    void Set_filter_in(LogLevel level);
    void Set_filter_out(LogLevel level);
    void Set_max_buffer(uint size);
    void Set_filename(std::string filename);
    uint Get_count();
    std::string Get_filename();
    ~FileSink();
};

}
} // namespace Wasi::Log

#endif