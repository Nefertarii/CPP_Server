#include "Head/logger.h"

using namespace Wasi::Log;

template <class... Args>
void Logger::Log(LogLevel level, Args... args) {
    if (filesink) {
    }
    if (stdsink) {
    }
}

Logger::Logger(std::string name_) :
    name(name_),
    error_handler(nullptr),
    filesink(nullptr),
    stdsink(nullptr) {}

/*
private:
    template <class... Args>
    void Log(LogLevel level, Args... args);

public:
    Logger(std::string name);
    template <class... Args>
    void Debug(Args... args);
    template <class... Args>
    void Info(Args... args);
    template <class... Args>
    void Warning(Args... args);
    template <class... Args>
    void Error(Args... args);
    template <class... Args>
    void Critical(Args... args);
    template <class... Args>
    void Fatal(Args... args);
    void Flush();
    void Flush_on(LogLevel level);
    void Set_formatter(LogFormat format);
    void Set_error_handler(ErrorHandler func);
    void Enable_std_sink();
    void Enable_file_sink();
    void Disenable_std_sink();
    void Disenable_file_sink();
    ~Logger();
*/