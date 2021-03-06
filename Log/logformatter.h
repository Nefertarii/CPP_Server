#ifndef LOG_FORMATTER_H_
#define LOG_FORMATTER_H_

#include "logmsg.h"
#include <map>

namespace Wasi {
namespace Log {

inline std::string FONT_COLOR_RESET          = "\033[0m";         /* Reset */
inline std::string FONT_COLOR_BLACK          = "\033[30m";        /* Black */
inline std::string FONT_COLOR_RED            = "\033[31m";        /* Red */
inline std::string FONT_COLOR_GREEN          = "\033[32m";        /* Green */
inline std::string FONT_COLOR_ORANGE         = "\033[33m";        /* ORANGE */
inline std::string FONT_COLOR_BLUE           = "\033[34m";        /* Blue */
inline std::string FONT_COLOR_MAGENTA        = "\033[35m";        /* Magenta */
inline std::string FONT_COLOR_CYAN           = "\033[36m";        /* Cyan */
inline std::string FONT_COLOR_WHITE          = "\033[37m";        /* White */
inline std::string FONT_COLOR_BOLDBLACK      = "\033[1m\033[30m"; /* Bold Black */
inline std::string FONT_COLOR_BOLDRED        = "\033[1m\033[31m"; /* Bold Red */
inline std::string FONT_COLOR_BOLDGREEN      = "\033[1m\033[32m"; /* Bold Green */
inline std::string FONT_COLOR_BOLDORANGE     = "\033[1m\033[33m"; /* Bold ORANGE */
inline std::string FONT_COLOR_BOLDBLUE       = "\033[1m\033[34m"; /* Bold Blue */
inline std::string FONT_COLOR_BOLDMAGENTA    = "\033[1m\033[35m"; /* Bold Magenta */
inline std::string FONT_COLOR_BOLDCYAN       = "\033[1m\033[36m"; /* Bold Cyan */
inline std::string FONT_COLOR_BOLDWHITE      = "\033[1m\033[37m"; /* Bold White */
inline std::string FONT_COLOR_INVERSEBLACK   = "\033[7m\033[30m"; /* Inverse Black */
inline std::string FONT_COLOR_INVERSERED     = "\033[7m\033[31m"; /* Inverse Red */
inline std::string FONT_COLOR_INVERSEGREEN   = "\033[7m\033[32m"; /* Inverse Green */
inline std::string FONT_COLOR_INVERSEORANGE  = "\033[7m\033[33m"; /* Inverse ORANGE */
inline std::string FONT_COLOR_INVERSEBLUE    = "\033[7m\033[34m"; /* Inverse Blue */
inline std::string FONT_COLOR_INVERSEMAGENTA = "\033[7m\033[35m"; /* Inverse Magenta */
inline std::string FONT_COLOR_INVERSECYAN    = "\033[7m\033[36m"; /* Inverse Cyan */
inline std::string FONT_COLOR_INVERSEWHITE   = "\033[7m\033[37m"; /* Inverse White */

struct LogFormat {
    bool print_color;
    bool print_thread_id;
    bool print_source_location;
    const char* data_format;
    std::map<LogLevel, std::string> consoles_color;
    LogFormat();
};

class LogFormatter {
private:
    std::string tmp_logmsg;
    LogFormat log_format;
    std::string process_date(long timestamp_ms, const char* format);

public:
    // [Thread:1000][FILE,FUNC,LINE][2022-3-23 21:00:00.000][LEVEL] print log
    LogFormatter();
    std::string Format(LogMsg& logmsg);
    void Format(LogMsg& logmsg, std::string& dest);
    void Set_format(LogFormat log_format_);
    ~LogFormatter();
};

}
} // namespace Wasi::Log

#endif