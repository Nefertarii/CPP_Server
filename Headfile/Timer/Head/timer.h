#ifndef TIMER_H_
#define TIMER_H_

#include "timestamp.h"
#include <string>

namespace Wasi {
    namespace Time {
        using Us = std::chrono::microseconds;
        using Ms = std::chrono::milliseconds;
        using Sec = std::chrono::seconds;
        using SysClock = std::chrono::system_clock;
        using HighResClock = std::chrono::high_resolution_clock;
        using SteadyClock = std::chrono::steady_clock;
        template <typename Rep, typename Period>
        using Duration = std::chrono::duration<Rep, Period>;
        template <typename type>
        using TimePoint = std::chrono::time_point<HighResClock, type>;

        class Timer {
        private:
            TimeStamp create_time;
            SteadyClock clock;
            Us Nowtime_us_();
        public:
            Timer();
            long Runtime_us();
            long Runtime_ms();
            long Runtime_sec();
            static long Nowtime_us();
            static long Nowtime_ms();
            static long Nowtime_sec();
            static std::string Sec_to_string(long time);
            //static std::string Ms_to_string(long time);
            //static std::string Us_to_string(long time);
            void Reset();
            ~Timer() {};
        };
    }
}


#endif