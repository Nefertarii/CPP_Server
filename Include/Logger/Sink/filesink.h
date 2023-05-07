#ifndef LOGGER_FILESINK_H_
#define LOGGER_FILESINK_H_

#include "Include/Logger/Sink/logsink.h"
#include "Include/Header/filehandler.h"
#include <vector>
#include <mutex>
#include <atomic>

namespace wasi {
namespace log {

class FileSink : public LogSink {
private:
    static std::atomic<int> create_num_;
    std::mutex mtx_;
    size_t suc_count_;
    std::vector<std::string> logs_;
    head::FileHandler filehandler_;
    void FileOut();
    void FileInit(std::string filename);

public:
    FileSink(std::string filename);
    void Insert(std::string& logmsg);
    void Flush();
    size_t GetCount();
    std::string GetName();
    ~FileSink() override;
};

}
} // namespace wasi::log

#endif