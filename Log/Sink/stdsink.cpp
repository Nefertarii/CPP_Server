#include "Head/stdsink.h"
#include "../../Class/exception.h"
#include "../Base/Head/fontcolor.h"
#include <iostream>

using namespace Wasi::Log;

void StdSink::Stdout(std::string message) {
    mtx.lock();
    os << message;
    ++count;
    mtx.unlock();
}

StdSink::StdSink() :
    count(0),
    os(&file) {
    LogFormat format;
    format.print_source_location = true;
    format.print_thread_id       = true;
    formatter.Set_format(format);
    if (!file.open("/dev/tty", std::ios::out)) {
        throw Exception("can't open /dev/tty\n");
    }
}

StdSink::StdSink(LogFormat format) :
    count(0),
    os(&file) {
    formatter.Set_format(format);
    if (!file.open("/dev/tty", std::ios::out)) {
        throw Exception("can't open /dev/tty\n");
    }
}

void StdSink::Logger(LogMsg& logmsg) {
    formatter.Format(logmsg);
    Stdout(logmsg.Output());
}

void StdSink::Flush() {
    // Stdout("Flush\n");
    os.flush();
}

void StdSink::Set_format(LogFormat fmt) { formatter.Set_format(fmt); }

uint StdSink::Get_count() { return count; }

StdSink::~StdSink() {
    file.close();
    // std::cout << "Total print " << count << " log message\n";
}