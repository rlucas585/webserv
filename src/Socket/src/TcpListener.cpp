/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpListener.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 22:00:56 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/03 15:22:21 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "TcpListener.hpp"
#include "../../Utils/src/Utils.hpp"
#include <cerrno>
#include <cstring>

TcpListener::TcpListener(void) : inner() {}

TcpListener::~TcpListener(void) {}

TcpListener::TcpListener(Socket sock) : inner(sock) {}

TcpListener::TcpListener(TcpListener const& other) { *this = other; }

TcpListener& TcpListener::operator=(TcpListener const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    inner = rhs.inner; // Move semantics preserved from FileDesc
    return *this;
}

// Requires updating for Ipv6
TcpListener::Result TcpListener::bind(const char* str) {
    SocketAddrV4::Result addr = SocketAddrV4::init(str);

    if (addr.is_err()) {
        return TcpListener::Result::Err(addr.unwrap_err());
    }
    return TcpListener::bind(addr.unwrap());
}

TcpListener::Result TcpListener::bind(SocketAddr const& addr) {
    // Get address info for later C-function calls
    Utils::pair<const sockaddr*, socklen_t> addr_info = addr.into_inner();
    const sockaddr* addrp = addr_info.first;
    socklen_t len = addr_info.second;

    // Create new socket
    Socket::Result sock_res = Socket::init(addr, SOCK_STREAM);

    if (sock_res.is_err())
        return TcpListener::Result::Err(sock_res.unwrap_err());
    Socket sock = sock_res.unwrap();

    // Set socket option: SO_REUSEADDR allows quick rebinding to socket, without
    // waiting for OS to clean up the previous one
    if (Socket::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 1) == -1) {
        return TcpListener::Result::Err(strerror(errno));
    }

    // Bind our new socket
    if (::bind(sock.into_inner(), addrp, len) == -1) {
        return TcpListener::Result::Err(strerror(errno));
    }

    // Start listening // 128 is the backlog (max length of pending connections)
    if (::listen(sock.into_inner(), 128) == -1) {
        return TcpListener::Result::Err(strerror(errno));
    }
    return TcpListener::Result::Ok(TcpListener(sock));
}

Socket const& TcpListener::socket(void) const { return inner; }

// Would need to be capable of returning SocketAddr instead of SocketAddrV4
// for Ipv6 functionality
TcpListener::AcceptResult TcpListener::accept(void) const {
    sockaddr_storage storage;
    socklen_t len = 0;

    Utils::memset(reinterpret_cast<void*>(&storage), 0, sizeof(storage));
    Socket::Result sock_res = inner.accept(&storage, &len);

    if (sock_res.is_err())
        return TcpListener::AcceptResult::Err(sock_res.unwrap_err());
    Socket sock = sock_res.unwrap();

    SocketAddrV4 addr = SocketAddrV4::init(*reinterpret_cast<sockaddr_in*>(&storage));
    return Utils::make_pair(TcpStream(sock), addr);
}
