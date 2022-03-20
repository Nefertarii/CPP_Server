#ifndef BASE_LOGLINE_H_
#define BASE_LOGLINE_H_

#include <string>

namespace Wasi {
namespace Log {

class LogLine {
private:
    std::string level;
    std::string date;
    std::string info;
    std::string detail;
    std::string process(std::string& str, char stop);

public:
    LogLine(std::string log); //[level][date] info:detail
    LogLine(std::string level, std::string date,
            std::string info, std::string detail);
    std::string& Change_level();
    std::string& Change_date();
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