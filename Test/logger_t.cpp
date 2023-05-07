#include "Include/Logger/logger.h"
#include <iostream>

using namespace std;
using namespace wasi;
using namespace wasi::log;

void Logger_t() {
    Logger::Init();
    LOG_DEBUG("Debug log");
    LOG_INFO("Info log");
    LOG_WARN("Warning log");
    LOG_ERROR("Error log");
    LOG_CRIT("Critical log");
    LOG_FATAL("Fatal log");
    Logger::AddFilterLevel(LogLevel::INFO);
    LOG_INFO("Info log 2");
    Logger::CancelFilterLevel(LogLevel::INFO);
    LOG_INFO("Info log 2");
    shared_ptr<ConsolesSink> sink = make_shared<ConsolesSink>();
    Logger::AddSink(sink);
    vector<string> sink_list = Logger::GetSinksName();
    for (auto i : sink_list) {
        cout << i << "\n";
    }
    LOG_INFO("Info log 3");
    Logger::RemoveSink(sink_list[1]);
    LOG_INFO("Info log 4");
    cout << "\n";
}

int main() {
    Logger_t();
}