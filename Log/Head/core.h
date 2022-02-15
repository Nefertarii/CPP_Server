#ifndef LOG_CORE_H_
#define LOG_CORE_H_

#include "formatter.h"
#include "sink.h"
#include "record.h"

namespace Wasi {
    namespace Log {
        class Core {
        private:
            Timer clock;
            Formatter formatter;
            Sink sink;
            Record record;
            uint log_size;
            uint log_capacity;
            std::queue<std::string> logs;
            std::queue<LogLine> logs_tmp;
        public:
            Core();
            Core(bool flag, uint capacity, LogLevel in_filter, LogLevel out_filter, std::string filename);
            bool Add_log(LogLevel log_level, std::string log_from, std::string log_detail);
            void Set_log_in_filter(LogLevel level);
            void Set_log_out_filter(LogLevel level);
            void Set_save_file(std::string filename);
            void Set_concurrency(bool flag);
            void Set_capacity(uint size);
            bool Save_to_file();
            ~Core() {}
        };
    }
}


/*
using LogNormal = 1;
using LogNotifi = 2;
using LogWarning = 3;
using LogError = 4;
using LogCritical = 5;
*/

#endif