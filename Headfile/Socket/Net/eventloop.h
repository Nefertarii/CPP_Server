#ifndef SOCKET_EVENTLOOP_H_
#define SOCKET_EVENTLOOP_H_

#include <assert.h>
#include <thread>
#include "../../Class/noncopyable.h"

namespace wasi {
    class Event_Loop : Noncopyable {
    private:
        void Abort_not_in_loop_thread();
        bool looping;
        const pid_t thread_id;
    public:
        Event_Loop();
        ~Event_Loop();
        void Loop();
        void Assert();
        bool Is_in_loop_thread();
    };
};






#endif