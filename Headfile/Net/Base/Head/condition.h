#ifndef NET_CONDITION_H_
#define NET_CONDITION_H_

#include <condition_variable>
#include <mutex>

namespace Wasi {
    namespace Net {
        class Condition {
        private:
            std::condition_variable cond;
            std::mutex mtx;
        public:
            Condition(std::mutex& mtx_);
            Wait();
            Wait_for();
            Wait_until();
            Notify();
            Notify_all();
            ~Condition();
        };
    }
}

#endif