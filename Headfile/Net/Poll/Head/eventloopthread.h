#ifndef NET_EVENT_LOOP_THREAD_H_
#define NET_EVENT_LOOP_THREAD_H_

#include "../../../Class/noncopyable.h"
#include "../../Base/thread.h"
#include <mutex>
#include <condition_variable>

namespace Wasi {
    namespace Net {
        class EventLoop;
        
        class EventLoopThread : Noncopyable {
        private:
            void Thread_func();
            EventLoop* loop;
            //std::thread thread;
        public:
            EventLoop* Start_loop();
        };
    }
}


#endif