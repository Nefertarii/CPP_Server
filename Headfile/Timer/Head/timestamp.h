#ifndef TIME_STAMP_H_
#define TIME_STAMP_H_

#include <chrono>
#include <sys/time.h>

namespace Wasi {
    namespace Time {
        class TimeStamp {
        private:
            long time; //MicroSeconds since epoch
        public:
            TimeStamp();
            TimeStamp(long microseconds);
            long Microseconds_since_epoch();
            static const int microseconds_per_second = 1000 * 1000;
            static const int microseconds_per_milliseconds = 1000;
        };
        
        TimeStamp Time_stamp_add(TimeStamp timestamp, double seconds);
        double Time_stamp_diff(TimeStamp high, TimeStamp low);
        inline bool operator<(TimeStamp lhs, TimeStamp rhs);
        inline bool operator==(TimeStamp lhs, TimeStamp rhs);
    }
}


#endif