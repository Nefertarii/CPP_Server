#include "Include/Logger/Sink/filesink.h"
#include "Include/Logger/header.h"
#include <iostream>

using namespace wasi;
using namespace wasi::log;

std::atomic<int> wasi::log::FileSink::create_num_(0);

void FileSink::FileOut() {
    size_t size = logs_.size();
    std::string log;
    for (size_t i = 0; i < size; i++) {
        log += logs_[i];
    }
    if (filehandler_.Write(log)) {
        logs_.clear();
        suc_count_ += size;
    }
    logs_.clear();
}

void FileSink::FileInit(std::string filename) {
    filehandler_.Create(filename);
}

FileSink::FileSink(std::string filename) :
    suc_count_(0) {
    std::lock_guard<std::mutex> lk(mtx_);
    name = "FileSink" + std::to_string(create_num_++);
    FileInit(filename);
}

void FileSink::Insert(std::string& logmsg) {
    std::lock_guard<std::mutex> lk(mtx_);
    logs_.push_back(logmsg);
    if (logs_.size() > 19) {
        FileOut();
    }
}

void FileSink::Flush() {
    std::lock_guard<std::mutex> lk(mtx_);
    FileOut();
}

size_t FileSink::GetCount() { return suc_count_; }

std::string FileSink::GetName() { return name; }

FileSink::~FileSink() {
    std::lock_guard<std::mutex> lk(mtx_);
    FileOut();
    filehandler_.Close();
}