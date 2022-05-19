#ifndef LOG_LOGGING_H_
#define LOG_LOGGING_H_

#include "Sink/filesink.h"
#include "Sink/stdsink.h"
#include "logger.h"
#include <unistd.h>

#ifdef NOPRINTDEBUG
#define LOG_DEBUG(detail) ;
#else
#define LOG_DEBUG(detail) Wasi::Log::Logging::Debug(detail, gettid(), Wasi::Log::Logging::Filename(__FILE__), __FUNCTION__, __LINE__);
#endif
#define LOG_INFO(detail) Wasi::Log::Logging::Info(detail, gettid(), Wasi::Log::Logging::Filename(__FILE__), __FUNCTION__, __LINE__);
#define LOG_WARN(detail) Wasi::Log::Logging::Warning(detail, gettid(), Wasi::Log::Logging::Filename(__FILE__), __FUNCTION__, __LINE__);
#define LOG_ERROR(detail) Wasi::Log::Logging::Error(detail, gettid(), Wasi::Log::Logging::Filename(__FILE__), __FUNCTION__, __LINE__);
#define LOG_CRITICAL(detail) Wasi::Log::Logging::Critical(detail, gettid(), Wasi::Log::Logging::Filename(__FILE__), __FUNCTION__, __LINE__);
#define LOG_FATAL(detail) Wasi::Log::Logging::Fatal(detail, gettid(), Wasi::Log::Logging::Filename(__FILE__), __FUNCTION__, __LINE__);

// std::make_shared<Wasi::Log::Logger>(default_logger_name, std::make_shared<Wasi::Log::FileSink>("test.log"));
// std::make_shared<Wasi::Log::Logger>(default_logger_name, std::make_shared<Wasi::Log::StdSink>());

namespace Wasi {
namespace Log {

class Logging {
private:
    static const char* default_logger_name;
    static std::mutex global_logger_mtx;
    static std::shared_ptr<Logger> default_logger;

public:
    static std::string Filename(std::string name);
    inline std::shared_ptr<Logger> Default_logger();
    static void Debug(std::string detail, int tid = 0, std::string file = "",
                      std::string func = "", int line = 0);
    static void Info(std::string detail, int tid = 0, std::string file = "",
                     std::string func = "", int line = 0);
    static void Warning(std::string detail, int tid = 0, std::string file = "",
                        std::string func = "", int line = 0);
    static void Error(std::string detail, int tid = 0, std::string file = "",
                      std::string func = "", int line = 0);
    static void Critical(std::string detail, int tid = 0, std::string file = "",
                         std::string func = "", int line = 0);
    static void Fatal(std::string detail, int tid = 0, std::string file = "",
                      std::string func = "", int line = 0);
    static void Change_default_logger(SinkPtr sink);
};

}
} // namespace Wasi::Log

#endif