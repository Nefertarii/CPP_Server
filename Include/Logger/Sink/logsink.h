#ifndef LOGGER_LOGSINK_H_
#define LOGGER_LOGSINK_H_

#include <string>

namespace wasi {
namespace log {
class LogMsg;

class LogSink {
protected:
    std::string name;

public:
    virtual void Insert(std::string& msg) = 0;
    virtual void Flush()                  = 0;
    virtual size_t GetCount()               = 0;
    virtual std::string GetName()         = 0;
    virtual ~LogSink()                    = default;
};

}
} // namespace wasi::log

#endif