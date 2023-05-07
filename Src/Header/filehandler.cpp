#include "Include/Header/filehandler.h"
#include <chrono>
#include <cstring>
#include <iostream>
#include <sys/stat.h>
#include <thread>

using namespace wasi;
using namespace wasi::head;

void FileHandler::OpenRead() {
    file_stream_.flush();
    file_stream_.close();
    file_stream_.open(file_name_, std::ios::binary | std::ios::in);
}

void FileHandler::OpenWrite() {
    file_stream_.flush();
    file_stream_.close();
    file_stream_.open(file_name_, std::ios::binary | std::ios::out | std::ios::in | std::ios::ate);
}

FileHandler::FileHandler() :
    open_tries_(5),
    open_interval_(1000) {
}

bool FileHandler::Open(std::string file_name, bool trunc) {
    file_name_ = file_name;
    if (file_stream_.is_open()) {
        Close();
    }
    for (uint i = 0; i < open_tries_; i++) {
        if (trunc) {
            file_stream_.open(file_name_.c_str());
        } else {
            file_stream_.open(file_name.c_str());
        }
        if (file_stream_) {
            file_stream_.close();
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(open_interval_));
    }
    return false;
}

bool FileHandler::Create(std::string file_name) {
    file_name_ = file_name;
    file_stream_.open(file_name, std::ios::binary | std::ios::in | std::ios::out);
    if (!file_stream_) {
        file_stream_.open(file_name, std::ios::trunc | std::ios::binary | std::ios::out);
        file_stream_.close();
        return true;
    }
    return false;
}

void FileHandler::Flush() { file_stream_.flush(); }

bool FileHandler::Write(const std::string& buf, size_t start) {
    OpenWrite();
    if (start != UINT_LEAST64_MAX) {
        file_stream_.seekp(start);
    }
    if (file_stream_.write(buf.c_str(), buf.size())) {
        Close();
        return true;
    } else {
        Close();
        return false;
    }
}

size_t FileHandler::Read(std::string& buf, size_t start, size_t size) {
    OpenRead();
    file_stream_.seekg(start);
    char tmp[size + 1] = {'\0'};
    file_stream_.read(tmp, size);
    buf = std::string(tmp);
    Close();
    return file_stream_.gcount();
}

size_t FileHandler::Read(char* buf, size_t start, size_t size) {
    OpenRead();
    file_stream_.seekg(start);
    file_stream_.read(buf, size);
    Close();
    return file_stream_.gcount();
}

void FileHandler::Close() {
    if (file_stream_) {
        file_stream_.flush();
        file_stream_.close();
    }
}

size_t FileHandler::GetFileSize() {
    struct stat stat_buf;
    int size = stat(file_name_.c_str(), &stat_buf);
    return size == 0 ? stat_buf.st_size : -1;
}

std::string FileHandler::GetFileName() const { return file_name_; }

size_t FileHandler::GetLine(std::string& buf, size_t line) {
    size_t row       = 1;
    size_t index     = 0;
    size_t file_size = GetFileSize();
    std::string tmp(file_size, ' ');
    size_t begin_posi = 0;
    size_t end_posi   = 0;
    size_t str_size   = tmp.size();
    index += Read(tmp, index, file_size);

    while (begin_posi < str_size) {
        end_posi = tmp.find_first_of('\n', begin_posi);
        if (row == line) {
            buf.assign(tmp, begin_posi, end_posi - begin_posi);
            return buf.size();
        }
        row++;
        begin_posi = end_posi + 1;
    }
    return 0;
}

size_t FileHandler::Find(std::string str) {
    return Find(str, 0);
}

size_t FileHandler::Find(std::string str, size_t start) {
    OpenRead();
    file_stream_.seekg(start);
    std::string line;
    size_t line_posi = start;
    while (std::getline(file_stream_, line, '\n')) {
        size_t n = line.find(str, 0);
        if (n != std::string::npos) {
            Close();
            return line_posi + n;
        }
        line_posi += line.size() + 1;
    }
    Close();
    return std::string::npos;
}

size_t FileHandler::Find(std::string str, size_t start, size_t length) {
    OpenRead();
    file_stream_.seekg(start);
    size_t read_num = length;
    std::string line;
    size_t line_posi = start;
    while (std::getline(file_stream_, line, '\n')) {
        size_t n = line.find(str, 0);
        if (n != std::string::npos) {
            Close();
            return line_posi + n;
        }
        line_posi += line.size() + 1;
        if (read_num -= line.size() + 1 > length) { return std::string::npos; }
    }
    Close();
    return std::string::npos;
}

FileHandler::~FileHandler() { Close(); }