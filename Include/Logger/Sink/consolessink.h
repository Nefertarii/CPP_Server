#ifndef LOGGER_CONSOLESSINK_H_
#define LOGGER_CONSOLESSINK_H_

#include "logsink.h"
#include <fstream>
#include <mutex>
#include <atomic>

namespace wasi {
namespace log {
class ConsolesSink : public LogSink {
private:
    std::mutex mtx_;
    std::atomic<uint> suc_count_;
    static std::atomic<int> create_num_;
    void ConsolesOut(std::string message);

public:
    ConsolesSink();
    void Insert(std::string& logmsg);
    void Flush();
    size_t GetCount();
    std::string GetName();
    ~ConsolesSink() override;
};

}
} // namespace wasi::log

#endif