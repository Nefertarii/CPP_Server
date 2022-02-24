#ifndef TIMER_TIMERID_H_
#define TIMER_TIMERID_H_

namespace Wasi {
    namespace Time {
        class Timer;
        class TimerId {
        private:
            Timer* timer;
            int sequence;
        public:
            TimerId();
            TimerId(Timer* timer);
            TimerId(Timer* timer_, int sequence_);
        };
    } // namespace Timer
} // namespace Wasi



#endif