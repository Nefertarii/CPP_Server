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
    std::string source_location; // file func line
    std::string formatted_msg;
    std::string Process(std::string& str, char stop);
    long Process(std::string& date);

public:
    LogMsg(std::string log); //[date][level]detail
    LogMsg(std::string date_, std::string level_, std::string detail_,
           int thread_id_ = 0, std::string source_location_ = "");
    LogMsg(long timestamp_ms, LogLevel level_, const char* detail_,
           int thread_id_ = 0, const char* source_location_ = "");
    void Format(std::string formatted_msg_);
    int& Get_thread_id();
    long& Get_date();
    std::string& Get_level();
    std::string& Get_detail();
    std::string& Get_source_location();
    std::string Output() const;
    ~LogMsg();
};

}
} // namespace Wasi::Log

#endif