#include "filehandler.h"
#include "exception.h"
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <thread>

using namespace Wasi;
using namespace Wasi::Base;

void FileHandler::Open_read() {
    file_stream.flush();
    file_stream.close();
    file_stream.open(file_name, std::ios::binary | std::ios::in);
}

void FileHandler::Open_write() {
    file_stream.flush();
    file_stream.close();
    file_stream.open(file_name, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);
}

FileHandler::FileHandler() :
    open_tries(5),
    open_interval(1000) {
    file_events.before_open  = nullptr;
    file_events.after_open   = nullptr;
    file_events.before_close = nullptr;
    file_events.after_close  = nullptr;
}

FileHandler::FileHandler(const FileEvents& filevents) :
    file_events(filevents),
    open_tries(5),
    open_interval(1000) {}

void FileHandler::Open(std::string file_name_, bool trunc) {
    if (file_stream.is_open()) { Close(); }
    file_name = file_name_;
    if (file_events.before_open) {
        file_events.before_open(file_name);
    }
    for (uint i = 0; i < open_tries; i++) {
        if (trunc) {
            file_stream.open(file_name.c_str());
        } else {
            file_stream.open(file_name.c_str());
        }
        if (file_stream) {
            if (file_events.after_open) {
                file_events.after_open(file_name, &file_stream);
            }
            file_stream.close();
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(open_interval));
        continue;
    }
    throw Exception("FileHandler() Failed open " + file_name + " for writing:", errno);
}

void FileHandler::Create(std::string file_name_) {
    file_name = file_name_;
    file_stream.open(file_name, std::ios::binary | std::ios::in | std::ios::out);
    if (!file_stream) {
        file_stream.open(file_name, std::ios::trunc | std::ios::binary | std::ios::out);
        file_stream.close();
    }
}

void FileHandler::Flush() {
    file_stream.flush();
}

void FileHandler::Write(const std::string& buf, size_t start) {
    Open_write();
    if (start != UINT_LEAST64_MAX) {
        file_stream.seekp(start);
    }
    if (file_stream.write(buf.c_str(), buf.size())) {
        Close();
        return;
    }
    throw Exception("FileHandler::Write() Failed Write " + file_name + "\n", errno);
}

size_t FileHandler::Read(std::string& buf, size_t start, size_t size) {
    Open_read();
    file_stream.seekg(start);
    char tmp[size + 1] = {0};
    file_stream.read(tmp, size);
    buf = std::string(tmp);
    Close();
    return file_stream.gcount();
}

void FileHandler::Close() {
    if (file_stream) {
        if (file_events.before_close) {
            file_events.before_close(file_name, &file_stream);
        }
        file_stream.close();
        if (file_events.after_close) {
            file_events.after_close(file_name);
        }
    }
}

size_t FileHandler::Get_file_size() {
    struct stat stat_buf;
    int size = stat(file_name.c_str(), &stat_buf);
    return size == 0 ? stat_buf.st_size : -1;
}

std::string FileHandler::Get_file_name() const { return file_name; }

size_t FileHandler::Get_line(std::string& buf, size_t line) {
    size_t row       = 1;
    size_t index     = 0;
    size_t file_size = Get_file_size();
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
    Open_read();
    file_stream.seekg(start);
    std::string line;
    size_t line_posi = start;
    while (std::getline(file_stream, line, '\n')) {
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
    Open_read();
    file_stream.seekg(start);
    size_t read_num = length;
    std::string line;
    size_t line_posi = start;
    while (std::getline(file_stream, line, '\n')) {
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