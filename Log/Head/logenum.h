#ifndef LOG_ENUM_H_
#define LOG_ENUM_H_

enum LogLevel {
    NONE = -1,
    NORMAL,
    NOTIFI,
    WARNING,
    ERROR,
    CRITICAL,
    LEVELEND = (1 << 30)
};

enum Location {
    L_CONSOLE,
    L_FILE,
    L_NETWORK,
    L_NONE,
    LOCATIONEND = (1 << 30)
};

#endif