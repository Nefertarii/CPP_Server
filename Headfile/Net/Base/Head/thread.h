#ifndef NET_THREAD_H_
#define NET_THREAD_H_

#include <thread>
#include <functional>
#include <atomic>
#include <latch>
#include "../../../Class/noncopyable.h"

namespace Wasi {
    namespace Net {
        using ThreadFunc = std::function<void()>;
        using Latch = std::latch;
        
        class Thread : Noncopyable {
        private:
            void Set_default_name();
            bool started;
            bool joined;
            pthread_t pthread_id;
            pid_t tid;
            std::string name;
            ThreadFunc func;
            Latch latch;
            static std::atomic<int> num_created;
        public:
            explicit Thread(ThreadFunc func_, const std::string& name_ = string());
            void Start();
            int Join();
            bool Started() const;
            pid_t Tid() const;
            const string& Name() const;
            static int Num_created();
            ~Thread();
        };
    }
}

#endif