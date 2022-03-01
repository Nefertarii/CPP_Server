#ifndef SOCKTES_ACCEPTOR_H_
#define SOCKTES_ACCEPTOR_H_

#include "../../../Class/noncopyable.h"
#include "../../Poll/Head/channel.h"
#include "inetaddress.h"

namespace Wasi {
    namespace Poll {
        class EventLoop;
    }
    
    namespace Sockets {

        using NewConnectCallback = std::function<void(int, const InetAddress&)>;
        
        class Acceptor : Noncopyable {
        private:
            void Handle_read();
            Poll::EventLoop* loop;
            Poll::Channel accept_channel;
            //Socket accept_socket;
            NewConnectCallback callback;
            bool listening;
            int idle_fd;
        public:
            Acceptor(Poll::EventLoop* loop_, const InetAddress& listen_addr, bool reuse_port);
            void Set_new_connection_callback(const NewConnectCallback& callback_);
            void Listen();
            bool Listening() const;
            ~Acceptor();
        };
    }
}

#endif