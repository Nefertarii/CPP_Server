#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_

#include "../../Class/noncopyable.h"
#include "timestamp.h"
#include <atomic>
#include <functional>

namespace Wasi {
namespace Time {

class Timer : Noncopyable {
private:
    const std::function<void()> callback;
    TimeStamp expiration;
    const double interval;
    const bool repeat;
    const int sequence;
    static std::atomic<int> create_num;

public:
    Timer(const std::function<void()>& callback_, TimeStamp when, double interval_);
    void Run() const;
    void Restart(TimeStamp now);
    bool Repeat();
    int Sequence();
    static int Create_num();
    TimeStamp Expiration();
};

}
} // namespace Wasi::Time

#endif // !TIMER_TIMER_H_