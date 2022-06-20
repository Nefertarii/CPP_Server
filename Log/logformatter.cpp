#include "logformatter.h"
#include "fontcolor.h"
#include <Base/Timer/clock.h>

using namespace Wasi;
using namespace Wasi::Log;

LogFormat::LogFormat() {
    print_color                             = true;
    print_thread_id                         = false;
    print_source_location                   = false;
    data_format                             = "%Y-%m-%d %H:%M:%S";
    consoles_color[LogLevel::UNINITIALIZED] = FONT_COLOR_WHITE;
    consoles_color[LogLevel::NONE]          = FONT_COLOR_WHITE;
    consoles_color[LogLevel::DBG]           = FONT_COLOR_BLUE;
    consoles_color[LogLevel::INFO]          = FONT_COLOR_GREEN;
    consoles_color[LogLevel::WARN]          = FONT_COLOR_ORANGE;
    consoles_color[LogLevel::ERR]           = FONT_COLOR_BOLDORANGE;
    consoles_color[LogLevel::CRITICAL]      = FONT_COLOR_RED;
    consoles_color[LogLevel::FATAL]         = FONT_COLOR_BOLDRED;
}

std::string LogFormatter::process_date(long timestamp_ms, const char* format) {
    long timestamp_sec = timestamp_ms / 1000;
    long time_ms       = timestamp_ms % 1000;
    char time[40];
    struct tm* time_tm = localtime(&timestamp_sec);
    strftime(time, 40, format, time_tm);
    std::string str_time = time;
    // if(str_time.size() == )
    if (time_ms < 10)
        return str_time + ".00" + std::to_string(time_ms);
    else if (time_ms < 100)
        return str_time + ".0" + std::to_string(time_ms);
    else
        return str_time + "." + std::to_string(time_ms);
}

LogFormatter::LogFormatter() :
    tmp_logmsg() {
    log_format.print_color                             = true;
    log_format.print_thread_id                         = false;
    log_format.print_source_location                   = false;
    log_format.data_format                             = "%Y-%m-%d %H:%M:%S";
    log_format.consoles_color[LogLevel::UNINITIALIZED] = FONT_COLOR_WHITE;
    log_format.consoles_color[LogLevel::NONE]          = FONT_COLOR_WHITE;
    log_format.consoles_color[LogLevel::DBG]           = FONT_COLOR_BLUE;
    log_format.consoles_color[LogLevel::INFO]          = FONT_COLOR_GREEN;
    log_format.consoles_color[LogLevel::WARN]          = FONT_COLOR_ORANGE;
    log_format.consoles_color[LogLevel::ERR]           = FONT_COLOR_BOLDORANGE;
    log_format.consoles_color[LogLevel::CRITICAL]      = FONT_COLOR_RED;
    log_format.consoles_color[LogLevel::FATAL]         = FONT_COLOR_BOLDRED;
}

std::string LogFormatter::Format(LogMsg& logmsg) {
    // [date]
    tmp_logmsg.clear();
    if (log_format.print_color) {
        tmp_logmsg += FONT_COLOR_GREEN;
    }
    tmp_logmsg += '[';
    tmp_logmsg += process_date(logmsg.Get_date(), log_format.data_format);
    tmp_logmsg += "] ";
    if (log_format.print_color) {
        tmp_logmsg += FONT_COLOR_RESET;
    }
    // [thread]
    if (log_format.print_color) {
        tmp_logmsg += FONT_COLOR_BLUE;
    }
    if (log_format.print_thread_id) {
        if (logmsg.Get_thread_id() != 0) {
            tmp_logmsg += "[Thread:";
            tmp_logmsg += std::to_string(logmsg.Get_thread_id());
            tmp_logmsg += "] ";
        }
    }
    if (log_format.print_color) {
        tmp_logmsg += FONT_COLOR_RESET;
    }
    // [file:line func()]
    if (log_format.print_source_location) {
        if (logmsg.Get_source_location().size() != 0) {
            if (log_format.print_color) {
                tmp_logmsg += FONT_COLOR_BLUE;
            }
            tmp_logmsg += '[';
            tmp_logmsg += logmsg.Get_source_location();
            tmp_logmsg += "] ";
            if (log_format.print_color) {
                tmp_logmsg += FONT_COLOR_RESET;
            }
        }
    }
    // [level]
    if (log_format.print_color) {
        tmp_logmsg += log_format.consoles_color[String_to_Level(logmsg.Get_level())];
    }
    tmp_logmsg += '[';
    tmp_logmsg += logmsg.Get_level();
    tmp_logmsg += "]";
    if (log_format.print_color) {
        tmp_logmsg += FONT_COLOR_RESET;
    }
    tmp_logmsg += ' ';
    // detail
    tmp_logmsg += logmsg.Get_detail();
    tmp_logmsg += "\n";
    logmsg.Format(tmp_logmsg);
    return tmp_logmsg;
}

void LogFormatter::Format(LogMsg& logmsg, std::string& dest) {
    dest = Format(logmsg);
}

void LogFormatter::Set_format(LogFormat log_format_) {
    log_format = log_format_;
}

LogFormatter::~LogFormatter() {}