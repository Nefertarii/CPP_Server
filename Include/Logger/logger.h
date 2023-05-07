#ifndef LOGGER_LOGGER_H_
#define LOGGER_LOGGER_H_

#include "Include/Logger/Sink/consolessink.h"
#include "Include/Logger/Sink/filesink.h"
#include "Include/Logger/appender.h"
#include <unistd.h>


#define LOG_DEBUG(content) wasi::log::Logger::Debug(content, gettid(), __FILE__, __FUNCTION__, __LINE__);
#define LOG_INFO(content) wasi::log::Logger::Info(content, gettid(), __FILE__, __FUNCTION__, __LINE__);
#define LOG_WARN(content) wasi::log::Logger::Warning(content, gettid(), __FILE__, __FUNCTION__, __LINE__);
#define LOG_ERROR(content) wasi::log::Logger::Error(content, gettid(), __FILE__, __FUNCTION__, __LINE__);
#define LOG_CRIT(content) wasi::log::Logger::Critical(content, gettid(), __FILE__, __FUNCTION__, __LINE__);
#define LOG_FATAL(content) wasi::log::Logger::Fatal(content, gettid(), __FILE__, __FUNCTION__, __LINE__);

// std::make_shared<Wasi::Log::Logger>(default_logger_name, std::make_shared<Wasi::Log::FileSink>("test.log"));
// std::make_shared<Wasi::Log::Logger>(default_logger_name, std::make_shared<Wasi::Log::StdSink>());

namespace wasi {
namespace log {
class Logger {
private:
    static std::string logger_name_;
    static Appender appender_;
    static std::mutex logger_mtx_;
public:
    Logger();
    static void Init();
    static size_t GetSize();
    static void AddSink(SinkPtr sink);
    static bool RemoveSink(std::string name);
    static std::vector<std::string> GetSinksName();
    static void AddFilterLevel(LogLevel level);
    static void CancelFilterLevel(LogLevel level);
    static void SetFormat(LogFormatter formatter); // change settings
    static void SetErrorHandler(ErrorHandler func);
    static void Debug(std::string content, int tid, std::string file, std::string func, int line);
    static void Info(std::string content, int tid, std::string file, std::string func, int line);
    static void Warning(std::string content, int tid, std::string file, std::string func, int line);
    static void Error(std::string content, int tid, std::string file, std::string func, int line);
    static void Critical(std::string content, int tid, std::string file, std::string func, int line);
    static void Fatal(std::string content, int tid, std::string file, std::string func, int line);
};

}} // namespace wasi::log

#endif
