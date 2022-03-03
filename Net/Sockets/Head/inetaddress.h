#ifndef SOCKTES_INETADDRESS_H_
#define SOCKTES_INETADDRESS_H_

#include <netinet/in.h>
#include <string>

namespace Wasi {
    namespace Sockets {
        class InetAddress {
        private:
            union {
                sockaddr_in addr;
                sockaddr_in6 addr6;
            };
            //Addr addr;
        public:
            explicit InetAddress(uint16_t port = 0, bool loopback = false, bool ipv6 = false);
            //1.2.3.4
            InetAddress(std::string ip, uint16_t port, bool ipv6 = false);
            explicit InetAddress(const sockaddr_in& addr_);
            explicit InetAddress(const sockaddr_in6& addr_);
            sa_family_t Family() const;
            std::string To_string_ip_port() const;
            std::string To_string_ip() const;
            uint16_t Port_net_endian() const;
            uint16_t Port() const;
            const sockaddr* Get_sockaddr() const;
            void Set_sockaddr_inet6(const sockaddr_in6& addr6_);
            void Set_scopeid(uint32_t scope_id); //ipv6
            uint32_t Net_endian_ipv4() const;
            uint16_t Net_endian_port() const;
            static bool Resolve(std::string host_name, InetAddress* result);
        };
    }
}


#endif