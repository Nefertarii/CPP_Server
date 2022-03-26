#include "Head/stdsink.h"
#include "../Base/Head/fontcolor.h"
#include <iostream>

using namespace Wasi::Log;

void StdSink::Stdout() {
    std::lock_guard<std::mutex> lk(mtx);
    if (filter_out == LogLevel::UNINITIALIZED || filter_out == LogLevel::NONE) {
        std::cout << logline.Output();
        logcount.fetch_add(1);
    } else if (String_to_Level(logline.Get_level()) != filter_out) {
        return;
    } else {
        std::cout << logline.Output();
        logcount.fetch_add(1);
    }
}

StdSink::StdSink(LogFormat logformat) :
    logcount(0),
    filter_in(LogLevel::UNINITIALIZED),
    filter_out(LogLevel::UNINITIALIZED) {
    formatter.Set_format(logformat);
}

void StdSink::Logger(LogMsg logmsg) {
    logline = logmsg;
    formatter.Format(logline);
    if (filter_in == LogLevel::UNINITIALIZED || filter_in == LogLevel::NONE) {
        Stdout();
    } else if (String_to_Level(logline.Get_level()) <= filter_in) {
        return;
    } else {
        Stdout();
    }
}

void StdSink::Flush() { std::cout << "flush" << std::endl; } //???

void StdSink::Set_format(LogFormat logformat) { formatter.Set_format(logformat); }

void StdSink::Set_filter_in(LogLevel level) { filter_in = level; }

void StdSink::Set_filter_out(LogLevel level) { filter_out = level; }

uint StdSink::Get_count() { return logcount.load(); }

StdSink::~StdSink() {
    std::cout << "Total print " << logcount << " log message\n";
}