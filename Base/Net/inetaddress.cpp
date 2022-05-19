#include "inetaddress.h"
#include "../Log/logging.h"
#include "socketapi.h"
#include <cassert>
#include <cstring>
#include <netdb.h>
#include <stddef.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace Wasi::Sockets;

InetAddress::InetAddress(uint16_t port, bool loopback, bool ipv6) {
    static_assert(offsetof(InetAddress, addr6) == 0, "addr_offset6 0");
    static_assert(offsetof(InetAddress, addr) == 0, "addr_offset 0");
    if (ipv6) {
        memset(&addr6, 0, sizeof(addr6));
        addr6.sin6_family = AF_INET6;
        in6_addr ip       = loopback ? in6addr_loopback : in6addr_any;
        addr6.sin6_addr   = ip;
        addr6.sin6_port   = Host_to_network_16(port);
    } else {
        memset(&addr, 0, sizeof(addr));
        addr.sin_family      = AF_INET;
        in_addr_t ip         = loopback ? INADDR_LOOPBACK : INADDR_ANY;
        addr.sin_addr.s_addr = Host_to_network_32(ip);
        addr.sin_port        = Host_to_network_16(port);
    }
}

InetAddress::InetAddress(std::string ip, uint16_t port, bool ipv6) {
    if (ipv6 || strchr(ip.c_str(), ':')) {
        memset(&addr6, 0, sizeof(addr6));
        From_ip_port(ip.c_str(), port, &addr6);
    } else {
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
    Wasi::Sockets::To_ip_port(buf, sizeof(buf), Get_sockaddr());
    return buf;
}

std::string InetAddress::To_string_ip() const {
    char buf[64] = "";
    Wasi::Sockets::To_ip(buf, sizeof(buf), Get_sockaddr());
    return buf;
}

uint16_t InetAddress::Port_net_endian() const {
    return addr.sin_port;
}

uint16_t InetAddress::Port() const {
    return Network_to_host_16(Port_net_endian());
}

const sockaddr* InetAddress::Get_sockaddr() const {
    return Wasi::Sockets::Sockaddr_cast(&addr6);
}

void InetAddress::Set_sockaddr_inet6(const sockaddr_in6& addr6_) {
    addr6 = addr6_;
}

void InetAddress::Set_scopeid(uint32_t scope_id) {
    if (Family() == AF_INET6) {
        addr6.sin6_scope_id = scope_id;
    }
}

uint32_t InetAddress::Net_endian_ipv4() const {
    assert(Family() == AF_INET);
    return addr.sin_addr.s_addr;
}

uint16_t InetAddress::Net_endian_port() const {
    return Network_to_host_16(Port_net_endian());
}

static thread_local char resolve_buffer[1024 * 64];

bool InetAddress::Resolve(std::string host_name, InetAddress* result) {
    assert(result != nullptr);
    hostent hentry;
    hostent* hentry2 = nullptr;
    int tmp_errno    = 0;
    memset(&hentry, 0, sizeof(hentry));
    int ret = gethostbyname_r(host_name.c_str(), &hentry, resolve_buffer,
                              sizeof(resolve_buffer), &hentry2, &tmp_errno);
    if (ret == 0 && hentry2 != nullptr) {
        assert(hentry2->h_addrtype == AF_INET && hentry2->h_length == sizeof(uint32_t));
        result->addr.sin_addr = *reinterpret_cast<in_addr*>(hentry2->h_addr_list[0]);
        return true;
    } else {
        if (ret) {
            LOG_ERROR("Error.");
        }
        return false;
    }
}
