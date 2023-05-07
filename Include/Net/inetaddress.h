#ifndef NET_INETADDRESS_H_
#define NET_INETADDRESS_H_

#include <netinet/in.h>
#include <string>

namespace wasi {
namespace net {

class InetAddress {
private:
    union {
        sockaddr_in addr_;
        sockaddr_in6 addr6_;
    };

public:
    explicit InetAddress(uint16_t port = 0, bool loopback = false, bool ipv6 = false);
    InetAddress(std::string ip, uint16_t port, bool ipv6 = false);
    explicit InetAddress(const sockaddr_in& addr);
    explicit InetAddress(const sockaddr_in6& addr6);
    sa_family_t GetFamily() const;
    std::string GetIpPort() const;
    std::string GetIp() const;
    uint32_t GetNetIpv4() const; // net endian ip inet4
    uint16_t GetNetPort() const; // net endian
    uint16_t GetPort() const;    // host endian
    const sockaddr* GetSockaddr() const;
    void SetSockaddrInet6(const sockaddr_in6& addr6);
    void SetScopeId(uint32_t scope_id);
    static bool Resolve(std::string host_name, InetAddress* result); // resolve hostnames to IPv4 addresses
    ~InetAddress();
};

}} // namespace wasi::net

#endif