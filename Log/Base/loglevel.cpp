#include "Head/loglevel.h"

namespace Wasi {
namespace Log {

LogLevel String_to_Level(std::string name) {
    if (name == "uninitialized" || name == "uinit") {
        return LogLevel::UNINITIALIZED;
    } else if (name == "none") {
        return LogLevel::NONE;
    } else if (name == "debug") {
        return LogLevel::DBG;
    } else if (name == "info") {
        return LogLevel::INFO;
    } else if (name == "warning" || name == "warn") {
        return LogLevel::WARN;
    } else if (name == "error" || name == "err") {
        return LogLevel::ERR;
    } else if (name == "critical") {
        return LogLevel::CRITICAL;
    } else if (name == "fatal") {
        return LogLevel::FATAL;
    } else {
        return LogLevel::UNINITIALIZED;
    }
}

std::string Level_to_string(LogLevel level) {
    if (level == LogLevel::UNINITIALIZED) {
        return "UNINITIALIZED";
    } else if (level == LogLevel::NONE) {
        return "NONE";
    } else if (level == LogLevel::DBG) {
        return "DEBUG";
    } else if (level == LogLevel::INFO) {
        return "INFO";
    } else if (level == LogLevel::WARN) {
        return "WARNING";
    } else if (level == LogLevel::ERR) {
        return "ERROR";
    } else if (level == LogLevel::CRITICAL) {
        return "CRITICAL";
    } else if (level == LogLevel::FATAL) {
        return "FATAL";
    } else {
        return "UNINITIALIZED";
    }
}

// std::ostream &operator<<(std::ostream &os, LogLevel level) {}

}
} // namespace Wasi::Log
