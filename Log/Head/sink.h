#ifndef LOG_SINK_H_
#define LOG_SINK_H_

#include <string>
#include <queue>
#include <mutex>
#include "logenum.h"
#include "../../Headfile/Timer/Head/timer.h"

namespace Wasi {
    namespace Log {
        class Sink {
        private:
            bool concurrency_flag;
            std::mutex mtx;
            std::queue<LogLine> log_queue;
            LogLevel in_filter;
            LogLevel out_filter;
            Wasi::Time::Timer clock;
            LogLevel Filter_str(std::string log_level);
        public:
            Sink();
            Sink(bool concurrency_flag_);
            Sink(LogLevel in, LogLevel out, bool concurrency_flag_);
            void Set_flag(bool concurrency_flag_);
            void Set_in_filter(LogLevel level);
            void Set_out_filter(LogLevel level);
            bool Log_add(std::string log_str);
            bool Log_add(LogLevel log_level, long log_date,
                std::string log_from, std::string log_detail);
            bool Log_consume(LogLine* log);
            bool Log_consume(std::queue<LogLine>* logs);
            std::string process(std::string& str, char stop);
        };
    }
}

#endif