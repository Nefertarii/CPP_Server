#ifndef HEADER_FILEHANDLER_H_
#define HEADER_FILEHANDLER_H_

#include <fstream>
#include <string>

namespace wasi {
namespace head {

class FileHandler {
private:
    std::string file_name_;
    std::fstream file_stream_;
    uint open_tries_;
    uint open_interval_; // ms
    void OpenRead();
    void OpenWrite();

public:
    FileHandler();
    bool Open(std::string file_name, bool trunc = false); // open file test
    bool Create(std::string file_name);                   // create a file
    void Flush();
    bool Write(const std::string& buf, size_t start = UINT_LEAST64_MAX);
    size_t Read(std::string& buf, size_t start = 0, size_t size = 4096);
    size_t Read(char* buf, size_t start = 0, size_t size = 4096);
    void Close();
    size_t GetFileSize();
    std::string GetFileName() const;
    // get specified line
    size_t GetLine(std::string& buf, size_t line);
    size_t Find(std::string str);
    size_t Find(std::string str, size_t start);
    // find str from 'start' max search 'length'
    size_t Find(std::string str, size_t start, size_t length);
    ~FileHandler();
};

}} // namespace wasi::head

#endif