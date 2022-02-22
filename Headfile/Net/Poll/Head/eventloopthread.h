#ifndef NET_EVENT_LOOP_THREAD_H_
#define NET_EVENT_LOOP_THREAD_H_

#include "../../../Class/noncopyable.h"
#include "../../Base/Head/thread.h"
#include <mutex>
#include <condition_variable>

namespace Wasi {
    namespace Net {
        class EventLoop;

        using ThreadInitCallback = std::function<void(EventLoop*)>;

        class EventLoopThread : Noncopyable {
        private:
            void Thread_func();
            EventLoop* loop;
            bool exiting;
            Thread thread;
            std::mutex mtx;
            std::mutex cond_mtx;
            std::condition_variable cond;
            ThreadInitCallback callback;
        public:
            /*
            EventLoopThread(const ThreadInitCallback& callback_ = ThreadInitCallback(),
                            const std::string& name = std::string());
            */
            EventLoopThread();
            EventLoop* Start_loop();
            ~EventLoopThread();

        };
    }
}


#endif