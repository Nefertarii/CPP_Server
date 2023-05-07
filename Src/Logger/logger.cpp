#include "Include/Logger/logger.h"

using namespace wasi;
using namespace wasi::log;

std::string Logger::logger_name_ = "default_logger";
Appender Logger::appender_("default_appender");
std::mutex Logger::logger_mtx_;

Logger::Logger() {}

void Logger::Init() {
    std::shared_ptr<FileSink> global_file_sink         = std::make_shared<FileSink>("/home/ubuntu/Server/Test/logger.txt");
    std::shared_ptr<ConsolesSink> global_consoles_sink = std::make_shared<ConsolesSink>();
    LogFormatter formatter;
    LoggerSetting setting;
    setting.print_color = false;
    formatter.SetFormat(setting);
    appender_.SetFormatter(formatter);
    appender_.PushBack(global_file_sink);
    appender_.PushBack(global_consoles_sink);
}

size_t Logger::GetSize() { return appender_.Size(); }

void Logger::AddSink(SinkPtr sink) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    appender_.PushBack(sink);
}

bool Logger::RemoveSink(std::string name) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    std::vector<std::string> sink_name_list = appender_.GetListName();
    for (size_t i = 0; i < sink_name_list.size(); i++) {
        if (sink_name_list[i] == name) {
            appender_.Remove(i);
            return true;
        }
    }
    return false;
}

std::vector<std::string> Logger::GetSinksName() { return appender_.GetListName(); }

void Logger::AddFilterLevel(LogLevel level) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    appender_.SetFilterLevel(level);
}

void Logger::CancelFilterLevel(LogLevel level) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    appender_.CancelFilterLevel(level);
}

void Logger::SetFormat(LogFormatter formatter) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    appender_.SetFormatter(formatter);
}

void Logger::SetErrorHandler(ErrorHandler func) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    appender_.SetErrorHandler(func);
}

void Logger::Debug(std::string content, int tid, std::string file, std::string func, int line) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    appender_.Log(LogLevel::DEBUG, content, tid, file, func, line);
}

void Logger::Info(std::string content, int tid, std::string file, std::string func, int line) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    appender_.Log(LogLevel::INFO, content, tid, file, func, line);
}

void Logger::Warning(std::string content, int tid, std::string file, std::string func, int line) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    appender_.Log(LogLevel::WARN, content, tid, file, func, line);
}

void Logger::Error(std::string content, int tid, std::string file, std::string func, int line) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    appender_.Log(LogLevel::ERR, content, tid, file, func, line);
}

void Logger::Critical(std::string content, int tid, std::string file, std::string func, int line) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    appender_.Log(LogLevel::CRIT, content, tid, file, func, line);
}

void Logger::Fatal(std::string content, int tid, std::string file, std::string func, int line) {
    std::lock_guard<std::mutex> lk(logger_mtx_);
    appender_.Log(LogLevel::FATAL, content, tid, file, func, line);
}
