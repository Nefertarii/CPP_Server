#ifndef SOCKTES_API_H_
#define SOCKTES_API_H_

#include <endian.h>
#include <string>
#include <arpa/inet.h>

namespace Wasi {
    namespace Sockets {
        inline uint64_t Host_to_network_64(uint64_t host64);
        inline uint32_t Host_to_network_32(uint64_t host32);
        inline uint16_t Host_to_network_16(uint64_t host16);

        inline uint64_t Network_to_host_64(uint64_t net64);
        inline uint32_t Network_to_host_32(uint64_t net32);
        inline uint16_t Network_to_host_16(uint64_t net16);

        int Accept(int sockfd, sockaddr_in6 addr);
        int Connect(int sockfd, const sockaddr* addr);
        ssize_t Read(int sockfd, void* buf, size_t count);
        //ssize_t Read_val(int sockfd, const iovec* iov, int iovcnt);
        ssize_t Write(int sockfd, const void* buf, size_t count);
        void Close(int sockfd);

        void From_ip_port(const char* ip, uint16_t port, sockaddr_in* addr);
        void From_ip_port(const char* ip, uint16_t port, sockaddr_in6* addr);
        void To_ip_port(char* buf, size_t size, const sockaddr* addr);
        void To_ip(char* buf, size_t size, const sockaddr* addr_);
        const sockaddr_in* sockaddr_in_cast(const sockaddr* addr);
        const sockaddr_in6* sockaddr_in6_cast(const sockaddr* addr);
        const sockaddr* sockaddr_cast(const sockaddr_in addr);
        const sockaddr* sockaddr_cast(const sockaddr_in6 addr);
        sockaddr* sockaddr_cast(const sockaddr_in6* addr);
    }
}



#endif