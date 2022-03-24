#include "Head/filehandler.h"
#include "../../Class/exception.h"
#include <cstring>
#include <iostream>
#include <sys/stat.h>

using namespace Wasi;
using namespace Wasi::Log;

FileHandler::FileHandler(const FileEvents& filevents) :
    file_events(filevents) {}

void FileHandler::Open(std::string file_name_, bool trunc) {
    Close();
    file_name = file_name_;
    if (file_events.before_open) {
        file_events.before_open(file_name);
    }
    for (uint i = 0; i < open_tries; i++) {
        if (trunc) {
            file_stream.open(file_name.c_str(), std::ios::trunc | std::ios::binary | std::ios::out);
        } else {
            file_stream.open(file_name.c_str(), std::ios::binary | std::ios::out);
        }
        if (file_stream) {
            if (file_events.after_open) {
                file_events.after_open(file_name, &file_stream);
            }
            return;
        }
        continue;
    }
    throw Exception("FileHandler() Failed open " + file_name + " for writing.");
}

void FileHandler::Reopen(bool trunc) {
    if (file_name.empty()) {
        throw Exception("FileHandler::Reopen() filename is empty.");
    }
    Open(file_name, trunc);
}

void FileHandler::Flush() {
    file_stream.flush();
}

void FileHandler::Write(const std::string& buf) {
    if (file_stream.write(buf.c_str(), buf.size())) {
        return;
    }
    throw Exception("FileHandler::Write() Failed Write " + file_name + "");
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

int FileHandler::Get_file_size() {
    struct stat stat_buf;
    size_t size = stat(file_name.c_str(), &stat_buf);
    return size == 0 ? stat_buf.st_size : -1;
}

std::string FileHandler::Get_file_name() const { return file_name; }

FileHandler::~FileHandler() { Close(); }