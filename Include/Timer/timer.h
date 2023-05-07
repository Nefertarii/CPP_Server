#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_

#include "timestamp.h"
#include <atomic>
#include <memory>
#include <functional>

namespace wasi {
namespace timer {

class Timer {
private:
    const std::function<void()> callback_;
    TimeStamp expiration_;               // when to execute the callback function
    const double interval_;              // timer execution interval
    const bool repeat_;                  //
    const int sequence_;                 // distinguish different timer objects
    static std::atomic<int> create_num_; // number of timer objects created

public:
    Timer(const std::function<void()>& callback, TimeStamp when, double interval);
    void Run() const;
    void Restart(TimeStamp now);
    bool Repeat();
    int Sequence();
    static int CreateNum();
    TimeStamp Expiration();
};

}} // namespace wasi::timer

#endif