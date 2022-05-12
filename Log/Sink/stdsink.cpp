#include "Head/stdsink.h"
#include "../../Class/exception.h"
#include "../Base/Head/fontcolor.h"
#include <iostream>

using namespace Wasi::Log;

void StdSink::Stdout(std::string message) {
    mtx.lock();
    std::cout << message;
    ++count;
    mtx.unlock();
}

StdSink::StdSink() :
    mtx(),
    count(0) {
    name = "Std Sink";
    LogFormat format;
    format.print_source_location = true;
    format.print_thread_id       = true;
    formatter.Set_format(format);
}

StdSink::StdSink(LogFormat format) :
    mtx(),
    count(0) {
    name = "Std Sink";
    formatter.Set_format(format);
}

void StdSink::Logger(LogMsg& logmsg) {
    formatter.Format(logmsg);
    Stdout(logmsg.Output());
}

void StdSink::Flush() {
    // Stdout("Flush\n");
    std::cout.flush();
}

void StdSink::Set_format(LogFormat fmt) { formatter.Set_format(fmt); }

uint StdSink::Get_count() { return count; }

StdSink::~StdSink() {
    // std::cout << "Total print " << count << " log message\n";
}