#ifndef SOCKTES_ACCEPTOR_H_
#define SOCKTES_ACCEPTOR_H_

#include "../../../Class/noncopyable.h"
#include "../../Poll/Head/channel.h"

namespace Wasi {
    namespace Poll {
        class EventLoop;
    }
    
    namespace Sockets {
        class Acceptor : Noncopyable {
        private:
            void Handle_read();
            Poll::EventLoop loop;
            Poll::Channel accept_channel;
            bool listening;
            int idle_fd;
        public:
            Acceptor(Poll::EventLoop* loop_, const)
        };
    }
}

#endif