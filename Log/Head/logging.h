#ifndef LOG_LOGGING_H_
#define LOG_LOGGING_H_

#include "../Sink/Head/stdsink.h"
#include "logger.h"

inline const char* default_logger_name = "";

#define LOG_DEBUG(detail) Wasi::Log::Debug(detail, gettid(), Wasi::Log::Filename(__FILE__), __FUNCTION__, __LINE__);
#define LOG_INFO(detail) Wasi::Log::Info(detail, gettid(), Wasi::Log::Filename(__FILE__), __FUNCTION__, __LINE__);
#define LOG_WARN(detail) Wasi::Log::Warning(detail, gettid(), Wasi::Log::Filename(__FILE__), __FUNCTION__, __LINE__);
#define LOG_ERROR(detail) Wasi::Log::Error(detail, gettid(), Wasi::Log::Filename(__FILE__), __FUNCTION__, __LINE__);
#define LOG_CRITICAL(detail) Wasi::Log::Critical(detail, gettid(), Wasi::Log::Filename(__FILE__), __FUNCTION__, __LINE__);
#define LOG_FATAL(detail) Wasi::Log::Fatal(detail, gettid(), Wasi::Log::Filename(__FILE__), __FUNCTION__, __LINE__);

namespace Wasi {
namespace Log {

inline std::string Filename(std::string name) {
    return std::string(name.begin() + name.find_last_of('/') + 1, name.end());
}

inline std::shared_ptr<Logger> default_logger() {
    std::shared_ptr<Logger> default_logger_;
    default_logger_ = std::make_shared<Logger>(default_logger_name, std::make_shared<StdSink>());
    return default_logger_;
}

inline void Debug(std::string detail, int tid = 0, std::string file = "",
                  std::string func = "", int line = 0) {
    default_logger()->Debug(detail, tid, file, func, line);
}

inline void Info(std::string detail, int tid = 0, std::string file = "",
                 std::string func = "", int line = 0) {
    default_logger()->Info(detail, tid, file, func, line);
}

inline void Warning(std::string detail, int tid = 0, std::string file = "",
                    std::string func = "", int line = 0) {
    default_logger()->Warning(detail, tid, file, func, line);
}

inline void Error(std::string detail, int tid = 0, std::string file = "",
                  std::string func = "", int line = 0) {
    default_logger()->Error(detail, tid, file, func, line);
}

inline void Critical(std::string detail, int tid = 0, std::string file = "",
                     std::string func = "", int line = 0) {
    default_logger()->Critical(detail, tid, file, func, line);
}

inline void Fatal(std::string detail, int tid = 0, std::string file = "",
                  std::string func = "", int line = 0) {
    default_logger()->Fatal(detail, tid, file, func, line);
}

}
} // namespace Wasi::Log

#endif