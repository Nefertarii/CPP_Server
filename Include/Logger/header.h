#ifndef LOGGER_HEADER_H_
#define LOGGER_HEADER_H_

#include <string>
#include <map>

namespace wasi {
namespace log {

const std::string FONT_COLOR_RESET          = "\033[0m";         /* Reset */
const std::string FONT_COLOR_BLACK          = "\033[30m";        /* Black */
const std::string FONT_COLOR_RED            = "\033[31m";        /* Red */
const std::string FONT_COLOR_GREEN          = "\033[32m";        /* Green */
const std::string FONT_COLOR_ORANGE         = "\033[33m";        /* ORANGE */
const std::string FONT_COLOR_BLUE           = "\033[34m";        /* Blue */
const std::string FONT_COLOR_MAGENTA        = "\033[35m";        /* Magenta */
const std::string FONT_COLOR_CYAN           = "\033[36m";        /* Cyan */
const std::string FONT_COLOR_WHITE          = "\033[37m";        /* White */
const std::string FONT_COLOR_BOLDBLACK      = "\033[1m\033[30m"; /* Bold Black */
const std::string FONT_COLOR_BOLDRED        = "\033[1m\033[31m"; /* Bold Red */
const std::string FONT_COLOR_BOLDGREEN      = "\033[1m\033[32m"; /* Bold Green */
const std::string FONT_COLOR_BOLDORANGE     = "\033[1m\033[33m"; /* Bold ORANGE */
const std::string FONT_COLOR_BOLDBLUE       = "\033[1m\033[34m"; /* Bold Blue */
const std::string FONT_COLOR_BOLDMAGENTA    = "\033[1m\033[35m"; /* Bold Magenta */
const std::string FONT_COLOR_BOLDCYAN       = "\033[1m\033[36m"; /* Bold Cyan */
const std::string FONT_COLOR_BOLDWHITE      = "\033[1m\033[37m"; /* Bold White */
const std::string FONT_COLOR_INVERSEBLACK   = "\033[7m\033[30m"; /* Inverse Black */
const std::string FONT_COLOR_INVERSERED     = "\033[7m\033[31m"; /* Inverse Red */
const std::string FONT_COLOR_INVERSEGREEN   = "\033[7m\033[32m"; /* Inverse Green */
const std::string FONT_COLOR_INVERSEORANGE  = "\033[7m\033[33m"; /* Inverse ORANGE */
const std::string FONT_COLOR_INVERSEBLUE    = "\033[7m\033[34m"; /* Inverse Blue */
const std::string FONT_COLOR_INVERSEMAGENTA = "\033[7m\033[35m"; /* Inverse Magenta */
const std::string FONT_COLOR_INVERSECYAN    = "\033[7m\033[36m"; /* Inverse Cyan */
const std::string FONT_COLOR_INVERSEWHITE   = "\033[7m\033[37m"; /* Inverse White */
// [2022-3-23 21:00:00.000][Thread:1000][FILE,FUNC,LINE][LEVEL] MSG

enum class LogLevel : uint32_t {
    UINIT = 0b0000000, // uninitialized.
    NONE  = 0b0000001, // special purpose.
    DEBUG = 0b0000010, // debug message.
    INFO  = 0b0000100, // normal output.
    WARN  = 0b0001000, // something fail, but whole program can continue run.
    ERR   = 0b0010000, // something crash, but whole program can continue run.
    CRIT  = 0b0100000, // main function will crash.
    FATAL = 0b1000000, // program crash.
};

inline LogLevel operator&(LogLevel lhs, LogLevel rhs) {
    return static_cast<LogLevel>(static_cast<uint>(lhs) & static_cast<uint>(rhs));
}

inline LogLevel operator|(LogLevel lhs, LogLevel rhs) {
    return static_cast<LogLevel>(static_cast<uint>(lhs) | static_cast<uint>(rhs));
}

inline LogLevel operator^(LogLevel lhs, LogLevel rhs) {
    return static_cast<LogLevel>(static_cast<uint>(lhs) ^ static_cast<uint>(rhs));
}

inline bool operator==(LogLevel lhs, uint rhs) {
    return static_cast<uint>(lhs) == rhs;
}

struct LoggerSetting {
    bool print_color;
    bool print_thread;
    bool print_location;
    const char* date_format;
    std::map<LogLevel, std::string> level_color;
    void SetDefault();
    LoggerSetting();
    LoggerSetting(const LoggerSetting& other);
    LoggerSetting& operator=(const LoggerSetting& other);
};

struct LogMsg {
    uint64_t date; // milliseconds
    int thread;
    std::string location;
    LogLevel level;
    std::string content;
    bool formatted;
    std::string str_date;
    std::string str_thread;
    std::string str_level;
    std::string logmsg;
    void Clear();
    LogMsg();
    LogMsg(const LogMsg& other);
    LogMsg& operator=(const LogMsg& other);
};

}} // namespace wasi::log

#endif