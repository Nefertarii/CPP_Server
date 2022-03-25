#include "Head/loglevel.h"

namespace Wasi {
namespace Log {

LogLevel String_to_Level(std::string level) {
    for (uint i = 0; i < level.size(); ++i) {
        if (65 <= level[i]) {
            if (level[i] <= 90) {
                level[i] += 32;
            }
        }
    }
    if (level == "uninitialized" || level == "uinit") {
        return LogLevel::UNINITIALIZED;
    } else if (level == "none") {
        return LogLevel::NONE;
    } else if (level == "debug") {
        return LogLevel::DBG;
    } else if (level == "info") {
        return LogLevel::INFO;
    } else if (level == "warning" || level == "warn") {
        return LogLevel::WARN;
    } else if (level == "error" || level == "err") {
        return LogLevel::ERR;
    } else if (level == "critical") {
        return LogLevel::CRITICAL;
    } else if (level == "fatal") {
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
