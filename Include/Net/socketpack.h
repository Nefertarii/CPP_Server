#ifndef NET_SOCKETPACK_H_
#define NET_SOCKETPACK_H_

#include <arpa/inet.h>

namespace wasi {
namespace net {

struct SocketPack {
    static int CreateSocket(sa_family_t family);
    static void Bind(int sockfd, const sockaddr* addr);
    static void Listen(int sockfd);
    static int Connect(int sockfd, const sockaddr* addr);
    static int Accept(int sockfd, sockaddr_in6* addr);
    static ssize_t Read(int sockfd, void* buf, size_t count);
    static ssize_t Write(int sockfd, const void* buf, size_t count);
    static void CloseWrite(int sockfd);
    static void CloseRead(int sockfd);
    static void Close(int sockfd);

    static int GetSocketError(int sockfd);
    static const char* StringError(int err); // errno to string
    static sockaddr_in6 GetLocalAddr(int sockfd);
    static sockaddr_in6 GetPeerAddr(int sockfd);
    // convert ip and port to sockaddr_in
    static void FromIpPort(const char* ip, uint16_t port, sockaddr_in* addr);
    // convert ip and port to sockaddr_in6
    static void FromIpPort(const char* ip, uint16_t port, sockaddr_in6* addr);
    // convert sockaddr ip and port to string(ip:port)
    static void ToIpPort(char* buf, size_t size, const sockaddr* addr);
    // convert sockaddr ip to string
    static void ToIp(char* buf, size_t size, const sockaddr* addr_);
    // convert sockaddr to sockaddr_in
    static const sockaddr_in* SockaddrInCast(const sockaddr* addr);
    // convert sockaddr to sockaddr_in6
    static const sockaddr_in6* SockaddrIn6Cast(const sockaddr* addr);
    // convert sockaddr_in to sockaddr
    static const sockaddr* SockaddrCast(const sockaddr_in* addr);
    // convert sockaddr_in6 to sockaddr
    static const sockaddr* SockaddrCast(const sockaddr_in6* addr);
    static sockaddr* SockaddrCast(sockaddr_in6* addr);

    static inline uint64_t HostToNetwork64(uint64_t host64) { return htobe64(host64); }
    static inline uint32_t HostToNetwork32(uint32_t host32) { return htobe32(host32); }
    static inline uint16_t HostToNetwork16(uint16_t host16) { return htobe16(host16); }
    static inline uint64_t NetworkToHost64(uint64_t net64) { return be64toh(net64); }
    static inline uint32_t NetworkToHost32(uint32_t net32) { return be32toh(net32); }
    static inline uint16_t NetworkToHost16(uint16_t net16) { return be16toh(net16); }

};

}} // namespace wasi::net

#endif