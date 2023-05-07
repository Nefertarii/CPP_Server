#ifndef HEADER_FILESTAT_H_
#define HEADER_FILESTAT_H_

#include <string>

namespace wasi {
namespace head {

class FileStat {
public:
    std::string target_path;
    std::string target_type;
    int file_fd;
    long file_offset;
    long file_length;
    long last_modify;
    FileStat();
    FileStat(const FileStat& other);
    FileStat& operator=(const FileStat& other);
    bool SetFileStat(std::string file_path = "\0");
    // find the assigned variable in "other" and replace "this"
    void SetExistFields(const FileStat& other);
    long Remaining();
    void Clear();
};

}} // namespace wasi::head

#endif