#include "Head/stdsink.h"
#include "Head/fontcolor.h"
#include <iostream>

using namespace Wasi::Log;

void StdSink::Stdout() {
    // std::cout << sink_settings->consoles_color[]
}

StdSink::StdSink(LogConfig* sink_settings_);

void StdSink::Logger(const LogMsg& logmsg);

void StdSink::Flush();

StdSink::~StdSink();