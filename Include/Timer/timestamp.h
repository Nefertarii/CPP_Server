#ifndef TIMER_TIMESTAMP_H_
#define TIMER_TIMESTAMP_H_

namespace wasi {
namespace timer {

// create time should be milliseconds
class TimeStamp {
private:
    long time_; // suggest use milliseconds since epoch
public:
    TimeStamp();
    TimeStamp(long microseconds);
    // add some time(second) to this timestamp(arg1)
    static TimeStamp TimeStampAdd(TimeStamp timestamp, double seconds);
    static double TimeStampDiff(TimeStamp high, TimeStamp low);
    // add seconds
    void TimeAdd(double seconds);
    // diff seconds
    double TimeDiff(TimeStamp other);
    long GetTime();
    void Swap(TimeStamp& other);
    bool Valid() const;
    static TimeStamp Invalid();
    static const int microseconds_per_second;
    static const int microseconds_per_milliseconds;
    static const int millisecond_per_second;
};

bool operator<(TimeStamp lhs, TimeStamp rhs);
bool operator==(TimeStamp lhs, TimeStamp rhs);

}} // namespace wasi::timer

#endif