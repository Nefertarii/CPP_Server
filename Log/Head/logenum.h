#ifndef LOG_ENUM_H_
#define LOG_ENUM_H_

enum LogLevel {
    LOG_NONE = -1,
    LOG_NORMAL,
    LOG_NOTIFI,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

enum Location {
    LOC_CONSOLE,
    LOC_FILE,
    LOC_NETWORK,
    LOC_NONE
};

#endif