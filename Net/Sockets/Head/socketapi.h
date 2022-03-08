#ifndef SOCKETS_API_H_
#define SOCKETS_API_H_

#include <arpa/inet.h>

namespace Wasi {
    namespace Sockets {
        inline uint64_t Host_to_network_64(uint64_t host64) { return htobe64(host64); }
        inline uint32_t Host_to_network_32(uint64_t host32) { return htobe32(host32); }
        inline uint16_t Host_to_network_16(uint64_t host16) { return htobe16(host16); }
        inline uint64_t Network_to_host_64(uint64_t net64) { return be64toh(net64); }
        inline uint32_t Network_to_host_32(uint64_t net32) { return be32toh(net32); }
        inline uint16_t Network_to_host_16(uint64_t net16) { return be16toh(net16); }

        int Create_socket(sa_family_t family);
        int Accept(int sockfd, sockaddr_in6* addr);
        int Connect(int sockfd, const sockaddr* addr);
        void Bind(int sockfd, const sockaddr* addr);
        void Listen(int sockfd);
        ssize_t Read(int sockfd, void* buf, size_t count);
        // ssize_t Read_val(int sockfd, const iovec* iov, int iovcnt);
        ssize_t Write(int sockfd, const void* buf, size_t count);
        void Shutdown_write(int sockfd);
        void Shutdown_read(int sockfd);
        void Close(int sockfd);

        int Get_socket_error(int sockfd);
        const char* String_error(int err);
        
        void From_ip_port(const char* ip, uint16_t port, sockaddr_in* addr);
        void From_ip_port(const char* ip, uint16_t port, sockaddr_in6* addr);
        void To_ip_port(char* buf, size_t size, const sockaddr* addr);
        void To_ip(char* buf, size_t size, const sockaddr* addr_);
        const sockaddr_in* Sockaddr_in_cast(const sockaddr* addr);
        const sockaddr_in6* Sockaddr_in6_cast(const sockaddr* addr);
        const sockaddr* Sockaddr_cast(const sockaddr_in* addr);
        const sockaddr* Sockaddr_cast(const sockaddr_in6* addr);
        sockaddr* Sockaddr_cast(sockaddr_in6* addr);
    }
}


#endif