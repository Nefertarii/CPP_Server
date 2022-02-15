#ifndef NET_EVENTLOOP_H_
#define NET_EVENTLOOP_H_

#include <condition_variable>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <assert.h>
#include "../../../Class/noncopyable.h"

#include <iostream>

namespace Wasi {
    namespace Socket {
        class Event_Loop : Noncopyable {
        private:
            bool looping;
            const pid_t thread_id;
        public:
            Event_Loop();
            void Loop();
            bool Is_in_loop_thread();
            ~Event_Loop();
        };
    }
}






#endif