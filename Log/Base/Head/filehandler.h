#ifndef LOG_FILEHANDLER_H_
#define LOG_FILEHANDLER_H_

#include <functional>
#include <string>

namespace Wasi {
namespace Log {

struct FileEvents {
    std::function<void*(std::string& filename)> before_open;
    std::function<void*(std::string& filename, std::FILE* file_stream)> after_open;
    std::function<void*(std::string& filename, std::FILE* file_stream)> before_close;
    std::function<void*(std::string& filename)> after_close;
    FileEvents() :
        before_open(nullptr),
        after_open(nullptr),
        before_close(nullptr),
        after_close(nullptr) {}
};

class FileHandler {
private:
    FileEvents file_events;
    std::string filename;
    std::FILE* filefd;
    uint open_tries;
    uint open_interval;

public:
    FileHandler();
    void Open();
    void Reopen();
    void Flush();
    void Write();
    void Close();
};

}
} // namespace Wasi::Log

#endif