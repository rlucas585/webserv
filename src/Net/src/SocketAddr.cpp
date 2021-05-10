#include "SocketAddr.hpp"
#include "../../Utils/src/Utils.hpp"

SocketAddrV4::SocketAddrV4(void) {}

SocketAddrV4::~SocketAddrV4(void) {}

SocketAddrV4::SocketAddrV4(SocketAddrV4 const& other) { inner = other.inner; }

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

SocketAddrV4::SocketAddrV4(sockaddr_in const& storage) { inner = storage; }

SocketAddrV4 SocketAddrV4::init(Ipv4Addr ip, u_int16_t port) { return SocketAddrV4(ip, port); }

SocketAddrV4::Result SocketAddrV4::init(Slice const& socket_addr_str) {
    Slice::Split iter = socket_addr_str.split(":");
    Slice ip_str = iter.next();
    Slice port_str = iter.next();
    int port;

    if (socket_addr_str.count(':') != 1)
        return SocketAddrV4::Result::Err("Invalid Slice used for SocketAddrV4");

    if (!ip_str.isInitialized() || !port_str.isInitialized() || !iter.is_complete()) {
        return SocketAddrV4::Result::Err("Invalid Slice used for SocketAddrV4");
    }
    Ipv4Addr::Result addr = Ipv4Addr::init_from_string(ip_str);
    if (addr.is_err())
        return SocketAddrV4::Result::Err(addr.unwrap_err());
    port = Utils::atoi(port_str.raw());
    if (port < 0 || port > 65535) {
        return SocketAddrV4::Result::Err("Invalid port value");
    }
    return SocketAddrV4::Result::Ok(SocketAddrV4(addr.unwrap(), static_cast<u_int16_t>(port)));
}

SocketAddrV4 SocketAddrV4::init(sockaddr_in const& storage) { return SocketAddrV4(storage); }

Ipv4Addr const& SocketAddrV4::ip(void) const {
    // Possible as Ipv4Addr has identical data structure to in_addr
    return *(reinterpret_cast<Ipv4Addr const*>(&inner.sin_addr));
}

void SocketAddrV4::set_ip(Ipv4Addr new_ip) { inner.sin_addr = new_ip.into_inner(); }

u_int16_t SocketAddrV4::port(void) const { return IpAddr::network_to_host_short(inner.sin_port); }

void SocketAddrV4::set_port(u_int16_t new_port) {
    inner.sin_port = IpAddr::host_to_network_short(new_port);
}

Utils::pair<const sockaddr*, socklen_t> SocketAddrV4::into_inner(void) const {
    return Utils::make_pair(reinterpret_cast<const sockaddr*>(&inner), sizeof(inner));
}

int SocketAddrV4::family(void) const { return AF_INET; }

bool SocketAddrV4::operator==(SocketAddrV4 const& other) const { return inner == other.inner; }

bool SocketAddrV4::operator!=(SocketAddrV4 const& other) const { return !(*this == other); }

bool SocketAddrV4::operator==(sockaddr_in const& other) const { return inner == other; }

bool SocketAddrV4::operator!=(sockaddr_in const& other) const { return inner != other; }

bool operator==(sockaddr_in const& lhs, SocketAddrV4 const& rhs) { return rhs == lhs; }

bool operator!=(sockaddr_in const& lhs, SocketAddrV4 const& rhs) { return rhs != lhs; }

bool operator==(sockaddr_in const& lhs, sockaddr_in const& rhs) {
    bool values = lhs.sin_family == rhs.sin_family && lhs.sin_port == rhs.sin_port &&
                  lhs.sin_addr.s_addr == rhs.sin_addr.s_addr;
    for (size_t i = 0; i < 8; i++) {
        if (lhs.sin_zero[i] != rhs.sin_zero[i])
            return false;
    }
    return values;
}

bool operator!=(sockaddr_in const& lhs, sockaddr_in const& rhs) { return !(lhs == rhs); }
