#include "Include/Logger/Sink/consolessink.h"
#include "Include/Logger/header.h"
#include <iostream>

using namespace wasi;
using namespace wasi::log;

std::atomic<int> wasi::log::ConsolesSink::create_num_(0);

void ConsolesSink::ConsolesOut(std::string message) {
    std::cout << message;
    std::cout.flush();
    ++suc_count_;
}

ConsolesSink::ConsolesSink() :
    suc_count_(0) {
    std::lock_guard<std::mutex> lk(mtx_);
    name = "ConsolesSink" + std::to_string(create_num_++);
}

void ConsolesSink::Insert(std::string& logmsg) {
    std::lock_guard<std::mutex> lk(mtx_);
    ConsolesOut(logmsg);
}

void ConsolesSink::Flush() {
    std::lock_guard<std::mutex> lk(mtx_);
    // ConsolesOut("Flush\n");
    std::cout.flush();
}

size_t ConsolesSink::GetCount() { return suc_count_; }

std::string ConsolesSink::GetName() { return name; }

ConsolesSink::~ConsolesSink() {
    std::cout.flush();
    // std::cout << "Total print " << count << " log message\n";
}