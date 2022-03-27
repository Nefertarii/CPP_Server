#include "Head/stdsink.h"
#include "../../Class/exception.h"
#include "../Base/Head/fontcolor.h"
#include <iostream>

using namespace Wasi::Log;

void StdSink::Stdout(std::string message) {
    mtx.lock();
    os << message;
    count.fetch_add();
    mtx.unlock();
}

StdSink::StdSink() :
    count(0),
    os(&file) {
    LogFormat format;
    formatter->Set_format(format);
    file.open("/dev/tty", ios::out);
    if (!file) {
        throw Exception("can't open /dev/tty\n");
    }
}

StdSink::StdSink(LogFormat format) :
    count(0),
    os(&file) {
    formatter->Set_format(format);
    file.open("/dev/tty", ios::out);
    if (!file) {
        throw Exception("can't open /dev/tty\n");
    }
}

void StdSink::Logger(LogMsg& logmsg) {
    formatter->Format(logmsg);
    Stdout(logmsg.Output());
}

void StdSink::Flush() {
    Stdout("Flush\n");
    os.flush();
}

void StdSink::Set_format(LogFormat fmt) { formatter->Set_format(fmt); }

uint StdSink::Get_count() { return count.load(); }

StdSink::~StdSink() {
    file.close();
    std::cout << "Total process " << count->load() << " log message\n";
}