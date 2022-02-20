#ifndef NET_THREAD_H_
#define NET_THREAD_H_

#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include "../../../Class/noncopyable.h"

namespace Wasi {
    namespace Net {
        using ThreadFunc = std::function<void()>;
        
        class Thread : Noncopyable {
        private:
            void Set_default_name();
            bool started;
            bool joined;
            pthread_t pthread_id;
            pid_t tid;
            std::string name;
            ThreadFunc func;
            static std::atomic<int> num_created;
        public:
            explicit Thread(ThreadFunc func, const string& name_ = string());
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