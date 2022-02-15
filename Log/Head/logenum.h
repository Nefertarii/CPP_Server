#ifndef LOG_ENUM_H_
#define LOG_ENUM_H_

namespace Wasi {
    namespace Log {
        enum LogLevel {
            LOG_NONE = -1,
            LOG_NORMAL,
            LOG_NOTIFI,
            LOG_WARNING,
            LOG_ERROR,
            LOG_CRITICAL,
            LOG_END = (1 << 30)
        };

        struct LogLine {
            std::string level;
            std::string date;
            std::string from;
            std::string detail;
        };

        enum Location {
            LOC_CONSOLE,
            LOC_FILE,
            LOC_NETWORK,
            LOC_NONE
        };

    }
}

#endif