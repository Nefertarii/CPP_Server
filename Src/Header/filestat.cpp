#include "Include/Header/filestat.h"
#include "Include/Logger/logger.h"
#include <sys/stat.h>

using namespace wasi;
using namespace wasi::head;

FileStat::FileStat() :
    target_path("\0"),
    target_type("\0"),
    file_fd(0),
    file_offset(0),
    file_length(0),
    last_modify(0) {}

FileStat::FileStat(const FileStat& other) {
    this->target_path = other.target_path;
    this->target_type = other.target_type;
    this->file_fd     = other.file_fd;
    this->file_offset = other.file_offset;
    this->file_length = other.file_length;
    this->last_modify = other.last_modify;
}

FileStat& FileStat::operator=(const FileStat& other) {
    this->target_path = other.target_path;
    this->target_type = other.target_type;
    this->file_fd     = other.file_fd;
    this->file_offset = other.file_offset;
    this->file_length = other.file_length;
    this->last_modify = other.last_modify;
    return *this;
}

bool FileStat::SetFileStat(std::string file_path) {
    struct stat sys_file_stat;
    target_path = file_path;
    if (stat(target_path.c_str(), &sys_file_stat) < 0) {
        LOG_ERROR("can't find file:" + target_path);
        return false;
    } else {
        size_t posi = target_path.find_last_of('.');
        if (posi == std::string::npos) { return false; }
        std::string file_type = target_path.substr(posi, target_path.length() - posi);
        target_type           = file_type;
        file_length           = sys_file_stat.st_size;
        last_modify           = sys_file_stat.st_mtim.tv_sec;
        return true;
    }
}

void FileStat::SetExistFields(const FileStat& other) {
    if (!other.target_path.empty()) {
        this->target_path = other.target_path;
    }
    if (!other.target_type.empty()) {
        this->target_type = other.target_type;
    }
    if (other.file_fd != 0) {
        this->file_fd = other.file_fd;
    }
    if (other.file_offset != 0) {
        this->file_offset = other.file_offset;
    }
    if (other.file_length != 0) {
        this->file_length = other.file_length;
    }
    if (other.last_modify != 0) {
        this->last_modify = other.last_modify;
    }
}

long FileStat::Remaining() { return file_length - file_offset; }

void FileStat::Clear() {
    target_path.clear();
    target_type.clear();
    file_fd     = 0;
    file_offset = 0;
    file_length = 0;
    last_modify = 0;
}
