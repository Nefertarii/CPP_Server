#ifndef TIMER_TIMESTAMP_H_
#define TIMER_TIMESTAMP_H_

#include <sys/time.h>

namespace Wasi {
namespace Time {

class TimeStamp {
private:
    long time; // MicroSeconds since epoch
public:
    TimeStamp();
    TimeStamp(long microseconds);
    long Microseconds_since_epoch();
    void Swap(TimeStamp& other);
    bool Valid() const;
    static TimeStamp Invalid();
    static const int microseconds_per_second       = 1000 * 1000;
    static const int microseconds_per_milliseconds = 1000;
};

inline TimeStamp Time_stamp_add(TimeStamp timestamp, double seconds) {
    long delta = static_cast<long>(seconds * TimeStamp::microseconds_per_second);
    return TimeStamp(timestamp.Microseconds_since_epoch() + delta);
}

inline double Time_stamp_diff(TimeStamp high, TimeStamp low) {
    long difftime = high.Microseconds_since_epoch() - low.Microseconds_since_epoch();
    return static_cast<double>(difftime) / TimeStamp::microseconds_per_second;
}

inline bool operator<(TimeStamp lhs, TimeStamp rhs) {
    return lhs.Microseconds_since_epoch() < rhs.Microseconds_since_epoch();
}

inline bool operator==(TimeStamp lhs, TimeStamp rhs) {
    return lhs.Microseconds_since_epoch() == rhs.Microseconds_since_epoch();
}

}
} // namespace Wasi::Time

#endif // !TIMER_TIMERQUEUE_H_