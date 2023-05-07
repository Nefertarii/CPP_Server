#include "Include/Logger/header.h"

using namespace wasi;
using namespace wasi::log;

void LoggerSetting::SetDefault() {
    print_color                  = true;
    print_thread                 = true;
    print_location               = true;
    date_format                  = "%Y-%m-%d %H:%M:%S";
    level_color[LogLevel::UINIT] = FONT_COLOR_WHITE;
    level_color[LogLevel::NONE]  = FONT_COLOR_WHITE;
    level_color[LogLevel::DEBUG] = FONT_COLOR_BLUE;
    level_color[LogLevel::INFO]  = FONT_COLOR_GREEN;
    level_color[LogLevel::WARN]  = FONT_COLOR_ORANGE;
    level_color[LogLevel::ERR]   = FONT_COLOR_BOLDORANGE;
    level_color[LogLevel::CRIT]  = FONT_COLOR_RED;
    level_color[LogLevel::FATAL] = FONT_COLOR_BOLDRED;
}

LoggerSetting::LoggerSetting() { SetDefault(); }

LoggerSetting::LoggerSetting(const LoggerSetting& other) {
    print_color    = other.print_color;
    print_thread   = other.print_thread;
    print_location = other.print_location;
    date_format    = other.date_format;
    level_color    = other.level_color;
}

LoggerSetting& LoggerSetting::operator=(const LoggerSetting& other) {
    print_color    = other.print_color;
    print_thread   = other.print_thread;
    print_location = other.print_location;
    date_format    = other.date_format;
    level_color    = other.level_color;
    return *this;
}

void LogMsg::Clear() {
    date = 0;
    str_date.clear();
    thread = 0;
    str_thread.clear();
    location.clear();
    level = LogLevel::UINIT;
    str_level.clear();
    content.clear();
    formatted = false;
    logmsg.clear();
}

LogMsg::LogMsg() { Clear(); }

LogMsg::LogMsg(const LogMsg& other) {
    date       = other.date;
    str_date   = other.str_date;
    thread     = other.thread;
    str_thread = other.str_thread;
    location   = other.location;
    level      = other.level;
    str_level  = other.str_level;
    content    = other.content;
    formatted  = other.formatted;
    logmsg     = other.logmsg;
}

LogMsg& LogMsg::operator=(const LogMsg& other) {
    date       = other.date;
    str_date   = other.str_date;
    thread     = other.thread;
    str_thread = other.str_thread;
    location   = other.location;
    level      = other.level;
    str_level  = other.str_level;
    content    = other.content;
    formatted  = other.formatted;
    logmsg     = other.logmsg;
    return *this;
}

