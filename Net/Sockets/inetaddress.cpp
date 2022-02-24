#include "Head/inetaddress.h"
#include "Head/socketapi.h"
#include <stddef.h>
#include <cstring>
#include <cassert>

using namespace Wasi::Sockets;

InetAddress::InetAddress(uint16_t port, bool loopback, bool ipv6) {
    static_assert(offsetof(InetAddress, addr6) == 0, "addr_offset6 0");
    static_assert(offsetof(InetAddress, addr) == 0, "addr_offset 0");
    if (ipv6) {
        memset(&addr6, 0, sizeof(addr6));
        addr6.sin6_family = AF_INET6;
        in6_addr ip = loopback ? in6addr_loopback : in6addr_any;
        addr6.sin6_addr = ip;
        addr6.sin6_port = Host_to_network_16(port);
    }
    else {
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        in_addr_t ip = loopback ? INADDR_LOOPBACK : INADDR_ANY;
        addr.sin_addr.s_addr = Host_to_network_32(ip);
        addr.sin_port = Host_to_network_16(port);
    }
}

InetAddress::InetAddress(std::string ip, uint16_t port, bool ipv6 = false) {
    if (ipv6 || strchr(ip.c_str(), ':')) {
        memset(&addr6, 0, sizeof(addr6));
        From_ip_port(ip.c_str(), port, &addr6);
    }
    else {
        memset(&addr, 0, sizeof(addr));
        From_ip_port(ip.c_str(), port, &addr);
    }
}

InetAddress::InetAddress(const sockaddr_in& addr_) :
    addr(addr_) {}

InetAddress::InetAddress(const sockaddr_in6& addr_) :
    addr6(addr_) {}

sa_family_t InetAddress::Family() const {
    return addr.sin_family;
}

std::string InetAddress::To_string_ip_port() const {
    char buf[64] = "";
    To_ip_port(buf, sizeof(buf), Get_sockaddr());
    return buf;
}

std::string InetAddress::To_string_ip() const {
    char buf[64] = "";
    To_ip(buf, sizeof(buf), Get_sockaddr());
    return buf;
}

uint16_t InetAddress::Port_net_endian() const {
    return addr.sin_port
}

uint16_t InetAddress::Port() const {
    return Network_to_host_16(Port_net_endiadn());
}

const sockaddr* InetAddress::Get_sockaddr() const {
    return sockaddr_cast(&addr6);
}

void InetAddress::Set_sockaddr_inet6(const sockaddr_in6& addr6_) {
    addr6 = addr6_;
}

void InetAddress::Set_scopeid(uint32_t scope_id) {
    if (Family() == AF_INET6) {
        addr6.sin6_scope_id = scope_id;
    }
}

uint32_t InetAddress::Netendian_ipv4() const {
    assert(Family() == AF_INET);
    return addr.sin_addr.s_addr;
}

uint16_t InetAddress::Netendian_port() const {
    Network_to_host_16(Port_net_endian());
}

bool InetAddress::Resolve(std::string host_name, InetAddress* result) {
    assert(result != nullptr);
    //...
}

