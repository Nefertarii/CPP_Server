#ifndef BASE_LOGMSG_H_
#define BASE_LOGMSG_H_

#include "loglevel.h"
#include <string>

namespace Wasi {
namespace Log {

class LogMsg {
private:
    bool formatted;
    int thread_id;
    long date;
    std::string level;
    std::string detail;
    std::string source_location; // file:func::line
    std::string formatted_msg;
    std::string process(std::string& str, char stop);
    std::string process(long timestamp_ms);

public:
    LogMsg(std::string log); //[date][level] detail
    LogMsg(int thread_id_ = 0, std::string level,
           std::string date, std::string detail_,
           std::string source_location_ = "");
    LogMsg(int thread_id_ = 0, long timestamp_ms,
           LogLevel level, const char* detail_,
           const char* source_location_ = "");
    void Format(std::string formatted_msg_);
    std::string& Get_thread_id();
    std::string& Get_level();
    std::string& Get_date();
    std::string& Get_detail();
    std::string& Get_source_location();
    std::string Output() const;
    std::string ~LogMsg();
};

}
} // namespace Wasi::Log

#endif