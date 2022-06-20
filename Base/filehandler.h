#ifndef BASE_FILEHANDLER_H_
#define BASE_FILEHANDLER_H_

#include <fstream>
#include <functional>
#include <string>

namespace Wasi {
namespace Base {

struct FileEvents {
    std::function<void(std::string& filename)> before_open;
    std::function<void(std::string& filename, std::fstream* file_stream)> after_open;
    std::function<void(std::string& filename, std::fstream* file_stream)> before_close;
    std::function<void(std::string& filename)> after_close;
    FileEvents() :
        before_open(nullptr),
        after_open(nullptr),
        before_close(nullptr),
        after_close(nullptr) {}
};

class FileHandler {
private:
    FileEvents file_events;
    // std::filesystem::path file_path;
    std::string file_name;
    std::fstream file_stream;
    uint open_tries;
    uint open_interval; // ms
    inline void Open_read();
    inline void Open_write();

public:
    FileHandler();
    FileHandler(const FileEvents& filevents);
    void Open(std::string file_name_, bool trunc = false);
    void Create(std::string file_name_);
    void Flush();
    void Write(const std::string& buf, size_t start = UINT_LEAST64_MAX);
    size_t Read(std::string& buf, size_t start = 0, size_t size = 4096);
    void Close();
    size_t Get_file_size();
    std::string Get_file_name() const;
    // get specified line
    size_t Get_line(std::string& buf, size_t line);
    size_t Find(std::string str);
    size_t Find(std::string str, size_t start);
    // find str from 'start' max search 'length'
    size_t Find(std::string str, size_t start, size_t length);
    ~FileHandler();
};
}
} // namespace Wasi::Base

#endif // !BASE_FILEHANDLER_H_