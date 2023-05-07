#include "Include/Net/inetaddress.h"
#include "Include/Logger/logger.h"
#include "Include/Net/socketpack.h"
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

using namespace wasi;
using namespace wasi::net;

InetAddress::InetAddress(uint16_t port, bool loopback, bool ipv6) {
    static_assert(offsetof(InetAddress, addr6_) == 0, "addr_offset6 0");
    static_assert(offsetof(InetAddress, addr_) == 0, "addr_offset 0");
    if (ipv6) {
        memset(&addr6_, 0, sizeof(addr6_));
        addr6_.sin6_family = AF_INET6;
        in6_addr ip        = loopback ? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr   = ip;
        addr6_.sin6_port   = SocketPack::HostToNetwork16(port);
    } else {
        memset(&addr_, 0, sizeof(addr_));
        addr_.sin_family      = AF_INET;
        in_addr_t ip          = loopback ? INADDR_LOOPBACK : INADDR_ANY;
        addr_.sin_addr.s_addr = SocketPack::HostToNetwork32(ip);
        addr_.sin_port        = SocketPack::HostToNetwork16(port);
    }
}

InetAddress::InetAddress(std::string ip, uint16_t port, bool ipv6) {
    if (ipv6 || strchr(ip.c_str(), ':')) {
        memset(&addr6_, 0, sizeof(addr6_));
        SocketPack::FromIpPort(ip.c_str(), port, &addr6_);
    } else {
        memset(&addr_, 0, sizeof(addr_));
        SocketPack::FromIpPort(ip.c_str(), port, &addr_);
    }
}

InetAddress::InetAddress(const sockaddr_in& addr) :
    addr_(addr) {}

InetAddress::InetAddress(const sockaddr_in6& addr6) :
    addr6_(addr6) {}

sa_family_t InetAddress::GetFamily() const {
    return addr_.sin_family;
}

std::string InetAddress::GetIpPort() const {
    char buf[64] = "";
    SocketPack::ToIpPort(buf, sizeof(buf), GetSockaddr());
    std::string ip_port = buf;
    return ip_port;
}

std::string InetAddress::GetIp() const {
    char buf[64] = "";
    SocketPack::ToIp(buf, sizeof(buf), GetSockaddr());
    std::string ip_port = buf;
    return ip_port;
}

uint32_t InetAddress::GetNetIpv4() const {
    return addr_.sin_addr.s_addr;
}

uint16_t InetAddress::GetNetPort() const {
    return addr_.sin_port;
}

uint16_t InetAddress::GetPort() const {
    return SocketPack::NetworkToHost16(GetNetPort());
}

const sockaddr* InetAddress::GetSockaddr() const {
    return SocketPack::SockaddrCast(&addr6_);
}

void InetAddress::SetSockaddrInet6(const sockaddr_in6& addr6) {
    addr6_ = addr6;
}

void InetAddress::SetScopeId(uint32_t scope_id) {
    if (GetFamily() == AF_INET6) {
        addr6_.sin6_scope_id = scope_id;
    }
}

bool InetAddress::Resolve(std::string host_name, InetAddress* result) {
    if (result == nullptr) {
        LOG_ERROR("result is nullptr");
        return false;
    }
    hostent hentry;
    hostent* hentry2 = nullptr;
    char resolve_buffer[1024 * 64];
    int tmp_errno = 0;
    memset(&hentry, 0, sizeof(hentry));
    int ret = gethostbyname_r(host_name.c_str(), &hentry, resolve_buffer,
                              sizeof(resolve_buffer), &hentry2, &tmp_errno);
    if (ret == 0 && hentry2 != nullptr) {
        result->addr_.sin_addr = *reinterpret_cast<in_addr*>(hentry2->h_addr_list[0]);
        return true;
    } else {
        if (ret) {
            LOG_ERROR("Error.");
        }
        return false;
    }
}

InetAddress::~InetAddress() {}