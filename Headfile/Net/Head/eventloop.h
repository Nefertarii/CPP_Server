#ifndef NET_EVENTLOOP_H_
#define NET_EVENTLOOP_H_

#include <condition_variable>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <assert.h>
#include "../../Class/noncopyable.h"

#include <iostream>

class Channel;

namespace Wasi {
    namespace Net {
        class Event_Loop : Noncopyable {
        private:
            bool looping;
            const pid_t thread_id;
        public:
            Event_Loop();
            void Loop();
            bool IsInLoopThread();
            void UpdateChannel(Channel* channel);
            void RemoveChannel(Channel* channel);
            bool HasChannel(Channel* channel);
            ~Event_Loop();
        };
    }
}






#endif