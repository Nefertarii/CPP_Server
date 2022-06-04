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

public:
    FileHandler();
    FileHandler(const FileEvents& filevents);
    void Open(std::string file_name_, bool trunc = false);
    void Reopen(bool trunc = false);
    void Create(std::string file_name_);
    void Flush();
    void Write(const std::string& buf);
    size_t Read(std::string& buf, size_t start = 0, size_t size = 4096);
    size_t Get_line(std::string& buf, size_t line);
    void Close();
    size_t Get_file_size();
    std::string Get_file_name() const;
    ~FileHandler();
};
}
} // namespace Wasi::Base

#endif // !BASE_FILEHANDLER_H_