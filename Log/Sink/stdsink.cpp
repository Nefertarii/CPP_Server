#include "Head/stdsink.h"
#include "../Base/Head/fontcolor.h"
#include <iostream>

using namespace Wasi::Log;

void StdSink::Stdout() {
    std::lock_guard<std::mutex> lk(mtx);
    std::cout << logline;
    logline.clear();
    ++logcount;
}

StdSink::StdSink(LogFormat logformat) {
    formatter.Set_format(logformat);
}

void StdSink::Logger(LogMsg& logmsg) {
    logline = formatter.Format(logmsg);
}

void StdSink::Flush() {
    std::cout << "flust" << std::endl;
}

StdSink::~StdSink() {
    std::cout << "Total print " << logcount << " log message\n";
}