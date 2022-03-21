#ifndef BASE_LOGMSG_H_
#define BASE_LOGMSG_H_

#include <string>

namespace Wasi {
namespace Log {

class LogMsg {
private:
    std::string level;
    std::string date;
    std::string info;
    std::string detail;
    std::string process(std::string& str, char stop);

public:
    LogMsg(std::string log); //[level][date] info:detail
    LogMsg(std::string level, std::string date,
           std::string info, std::string detail);
    std::string& Change_level();
    std::string& Change_date();
    std::string& Change_info();
    std::string& Change_detail();
    ~LogMsg();
};

}
} // namespace Wasi::Log

#endif