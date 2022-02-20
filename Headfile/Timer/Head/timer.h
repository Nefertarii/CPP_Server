#ifndef TIME_TIMER_H_
#define TIME_TIMER_H_

#include <functional>
#include <atomic>
#include "timestamp.h"
#include "../../Class/noncopyable.h"

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
    } // namespace Time
} // namespace Wasi



#endif