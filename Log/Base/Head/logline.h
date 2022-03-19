#ifndef BASE_LOGLINE_H_
#define BASE_LOGLINE_H_

#include "loglevel.h"
#include <string>

namespace Wasi {
namespace Log {

class LogLine {
private:
    LogLevel level;
    long date;
    std::string info;
    std::string detail;
    std::string process(std::string& str, char stop);

public:
    LogLine(std::string log); //[level][date] info:detail
    LogLine(LogLevel level, longdate,
            std::string info, std::string detail);
    LogLevel& Change_level();
    long& Change_date();
    std::string& Change_info();
    std::string& Change_detail();
    ~LogLine();
};

}
} // namespace Wasi::Log

#endif

/*

namespace Wasi {
namespace Log {


} //namespace Wasi
} //namespace Log

*/