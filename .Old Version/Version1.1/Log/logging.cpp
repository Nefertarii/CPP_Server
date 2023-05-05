#include "logging.h"

using namespace Wasi::Log;

const char* Logging::default_logger_name = "";
std::mutex Logging::global_logger_mtx;
std::shared_ptr<Logger> Logging::default_logger = std::make_shared<Logger>(default_logger_name, std::make_shared<StdSink>());

std::string Logging::Filename(std::string name) {
    return std::string(name.begin() + name.find_last_of('/') + 1, name.end());
}

inline std::shared_ptr<Logger> Logging::Default_logger() {
    return Logging::default_logger;
}

void Logging::Debug(std::string detail, int tid, std::string file, std::string func, int line) {
    default_logger->Debug(detail, tid, file, func, line);
}

void Logging::Info(std::string detail, int tid, std::string file, std::string func, int line) {
    default_logger->Info(detail, tid, file, func, line);
}

void Logging::Warning(std::string detail, int tid, std::string file, std::string func, int line) {
    default_logger->Warning(detail, tid, file, func, line);
}

void Logging::Error(std::string detail, int tid, std::string file, std::string func, int line) {
    default_logger->Error(detail, tid, file, func, line);
}

void Logging::Critical(std::string detail, int tid, std::string file, std::string func, int line) {
    default_logger->Critical(detail, tid, file, func, line);
}

void Logging::Fatal(std::string detail, int tid, std::string file, std::string func, int line) {
    default_logger->Fatal(detail, tid, file, func, line);
}

void Logging::Change_default_logger(SinkPtr sink) {
    std::lock_guard<std::mutex> lk(Logging::global_logger_mtx);
    LOG_INFO("Change log");
    default_logger->Remove(0);
    default_logger->Push_back(sink);
}