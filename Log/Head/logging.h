#ifndef LOG_LOGGING_H_
#define LOG_LOGGING_H_

#include "../Sink/Head/stdsink.h"
#include "logger.h"

const char* default_logger_name = "";
Wasi::Log::Logger* default_logger();

namespace Wasi {
namespace Log {

Logger* default_logger() {
    std::shared_ptr<Logger> default_logger_;
    default_logger_ = std::make_shared<Logger>(default_logger_name, std::make_shared<StdSink>());
    return default_logger_.get();
}

void Debug(std::string detail, int tid = 0, std::string file = "",
           std::string func = "", int line = 0) {
    default_logger()->Debug(detail, tid, file, func, line);
}
void Info(std::string detail, int tid = 0, std::string file = "",
          std::string func = "", int line = 0) {
    default_logger()->Info(detail, tid, file, func, line);
}
void Warning(std::string detail, int tid = 0, std::string file = "",
             std::string func = "", int line = 0) {
    default_logger()->Warning(detail, tid, file, func, line);
}
void Error(std::string detail, int tid = 0, std::string file = "",
           std::string func = "", int line = 0) {
    default_logger()->Error(detail, tid, file, func, line);
}
void Critical(std::string detail, int tid = 0, std::string file = "",
              std::string func = "", int line = 0) {
    default_logger()->Critical(detail, tid, file, func, line);
}
void Fatal(std::string detail, int tid = 0, std::string file = "",
           std::string func = "", int line = 0) {
    default_logger()->Fatal(detail, tid, file, func, line);
}

}
} // namespace Wasi::Log

#endif