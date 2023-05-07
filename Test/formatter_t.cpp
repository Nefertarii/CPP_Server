#include "Include/Logger/formatter.h"

using namespace wasi;
using namespace wasi::log;

std::string wasi::log::LevelToString(LogLevel level) {
    switch (level) {
    case LogLevel::UINIT:
        return "\0";
    case LogLevel::NONE:
        return "\0";
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARN:
        return "WARNING";
    case LogLevel::ERR:
        return "ERROR";
    case LogLevel::CRIT:
        return "CRITICAL";
    case LogLevel::FATAL:
        return "FATAL";
    default:
        return "\0";
    }
}

LogLevel wasi::log::StringToLevel(std::string level) {
    for (uint i = 0; i < level.size(); ++i) {
        if (65 <= level[i]) {
            if (level[i] <= 90) {
                level[i] += 32;
            }
        }
    }
    if (level == "uninitialized" || level == "uinit") {
        return LogLevel::UINIT;
    } else if (level == "none") {
        return LogLevel::NONE;
    } else if (level == "debug") {
        return LogLevel::DEBUG;
    } else if (level == "info") {
        return LogLevel::INFO;
    } else if (level == "warning" || level == "warn") {
        return LogLevel::WARN;
    } else if (level == "error" || level == "err") {
        return LogLevel::ERR;
    } else if (level == "critical" || level == "crit") {
        return LogLevel::CRIT;
    } else if (level == "fatal") {
        return LogLevel::FATAL;
    } else {
        return LogLevel::UINIT;
    }
}

void LogFormatter::ProcessingDate() {
    long time_sec = tmplog_.date / 1000;
    long time_ms  = tmplog_.date % 1000;
    char time[40];
    struct tm* time_tm = localtime(&time_sec);
    strftime(time, 40, formater_.date_format, time_tm);
    tmplog_.str_date = '[';
    tmplog_.str_date += time;
    if (time_ms < 10)
        tmplog_.str_date += (".00" + std::to_string(time_ms));
    else if (time_ms < 100)
        tmplog_.str_date += (".0" + std::to_string(time_ms));
    else
        tmplog_.str_date += ("." + std::to_string(time_ms));
    tmplog_.str_date += ']';
}

void LogFormatter::ProcessingHead() {
    // [2022-3-23 21:00:00.000][Thread:1000][FILE,FUNC,LINE][LEVEL] MSG
    if (formater_.print_thread) {
        if (tmplog_.thread != 0) {
            tmplog_.str_thread = "[Thread:";
            tmplog_.str_thread += std::to_string(tmplog_.thread);
            tmplog_.str_thread += "]";
        }
    }
    if (formater_.print_location) {
        if (tmplog_.location.size() != 0) {
            tmplog_.location.insert(0, 1, '[');
            tmplog_.location += ']';
        }
    } else {
        tmplog_.location = "\0";
    }
    if (formater_.print_color) {
        tmplog_.str_level = formater_.level_color[tmplog_.level];
    }
    tmplog_.str_level += ('[' + LevelToString(tmplog_.level) + "] ");
    if (formater_.print_color) {
        tmplog_.str_level += FONT_COLOR_RESET;
    }
}

void LogFormatter::ProcessingMsg() {
    tmplog_.logmsg = tmplog_.str_date;
    tmplog_.logmsg += tmplog_.str_thread;
    tmplog_.logmsg += tmplog_.location;
    tmplog_.logmsg += tmplog_.str_level;
    tmplog_.logmsg += tmplog_.content;
    tmplog_.logmsg += "\n";
    tmplog_.formatted = true;
}

LogFormatter::LogFormatter() :
    tmpstr_() {
    formater_.SetDefault();
    tmplog_.Clear();
}

void LogFormatter::SetFormat(LoggerSetting setting) { formater_ = setting; }

void LogFormatter::Format(LogMsg& msg) {
    tmplog_ = msg;
    ProcessingDate();
    ProcessingHead();
    ProcessingMsg();
    msg = tmplog_;
}

void LogFormatter::Format(LogMsg& msg, std::string& dest) {
    Format(msg);
    dest = msg.logmsg;
}

LogFormatter::~LogFormatter() {}