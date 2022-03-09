#ifndef SOCKETS_SOCKET_H_
#define SOCKETS_SOCKET_H_

#include "../../../Class/noncopyable.h"
#include <netinet/tcp.h>
#include <string>

namespace Wasi {
    namespace Sockets {
        class InetAddress;

        class Socket : Noncopyable {
        private:
            const int sockfd;
        public:
            explicit Socket(int sockfd_);
            int Fd() const;
            void Bind_address(const InetAddress& localaddr);
            void Listen();
            int Accept(InetAddress* peeraddr);
            void Shutdown_write();
            void Set_tcp_delay(bool status);
            void Set_reuse_addr(bool status);
            void Set_reuse_port(bool status);
            void Set_keep_alive(bool status);
            bool Get_tcp_info(std::string* buf) const;
            bool Get_tcp_info(tcp_info* tcpi) const;
            ~Socket();
        };
    }
}



#endif