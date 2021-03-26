/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   SocketAddr.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/24 18:59:55 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/26 18:19:12 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "SocketAddr.hpp"
#include "../../Utils/src/Utils.hpp"

SocketAddrV4::SocketAddrV4(void) {}

SocketAddrV4::~SocketAddrV4(void) {}

SocketAddrV4::SocketAddrV4(SocketAddrV4 const& other) : inner(other.inner) {}

SocketAddrV4 SocketAddrV4::operator=(SocketAddrV4 const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    inner = rhs.inner;
    return *this;
}

SocketAddrV4::SocketAddrV4(Ipv4Addr ip, u_int16_t port) {
    inner.sin_family = AF_INET;
    inner.sin_port = IpAddr::host_to_network_short(port);
    inner.sin_addr = ip.into_inner();
    Utils::memset(reinterpret_cast<void*>(inner.sin_zero), 0, 8);
}

SocketAddrV4 SocketAddrV4::init(Ipv4Addr ip, u_int16_t port) { return SocketAddrV4(ip, port); }

Ipv4Addr const& SocketAddrV4::ip(void) const {
    // Possible as Ipv4Addr has identical data structure to in_addr
    return *(reinterpret_cast<Ipv4Addr const*>(&inner.sin_addr));
}

void SocketAddrV4::set_ip(Ipv4Addr new_ip) { inner.sin_addr = new_ip.into_inner(); }

u_int16_t SocketAddrV4::port(void) const { return IpAddr::network_to_host_short(inner.sin_port); }

void SocketAddrV4::set_port(u_int16_t new_port) { inner.sin_port = IpAddr::host_to_network_short(new_port); }

sockaddr_in SocketAddrV4::into_inner(void) const { return inner; }

bool SocketAddrV4::operator==(SocketAddrV4 const& other) const { return inner == other.inner; }

bool SocketAddrV4::operator!=(SocketAddrV4 const& other) const { return !(*this == other); }

bool SocketAddrV4::operator==(sockaddr_in const& other) const { return inner == other; }

bool SocketAddrV4::operator!=(sockaddr_in const& other) const { return inner != other; }

bool operator==(sockaddr_in const& lhs, SocketAddrV4 const& rhs) { return rhs == lhs; }

bool operator!=(sockaddr_in const& lhs, SocketAddrV4 const& rhs) { return rhs != lhs; }

bool operator==(sockaddr_in const& lhs, sockaddr_in const& rhs) {
    bool values =
        lhs.sin_family == rhs.sin_family && lhs.sin_port == rhs.sin_port && lhs.sin_addr.s_addr == rhs.sin_addr.s_addr;
    for (size_t i = 0; i < 8; i++) {
        if (lhs.sin_zero[i] != rhs.sin_zero[i])
            return false;
    }
    return values;
}

bool operator!=(sockaddr_in const& lhs, sockaddr_in const& rhs) { return !(lhs == rhs); }
