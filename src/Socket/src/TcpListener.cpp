/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpListener.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 22:00:56 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/31 15:00:28 by rlucas        ########   odam.nl         */
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
TcpListener TcpListener::bind(const char* str) { return TcpListener::bind(SocketAddrV4::init(str)); }

TcpListener TcpListener::bind(SocketAddr const& addr) {
    // Get address info for later C-function calls
    Utils::pair<const sockaddr*, socklen_t> addr_info = addr.into_inner();
    const sockaddr* addrp = addr_info.first;
    socklen_t len = addr_info.second;

    // Create new socket
    Socket sock = Socket::init(addr, SOCK_STREAM);
    int err = 0;

    // Set socket option: SO_REUSEADDR allows quick rebinding to socket, without
    // waiting for OS to clean up the previous one
    err = Socket::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 1);
    if (err == -1) {
        throw Utils::runtime_error(std::string("Error in setsockopt(): ") + strerror(errno));
    }

    // Bind our new socket
    err = ::bind(sock.into_inner(), addrp, len);
    if (err == -1) {
        throw Utils::runtime_error(std::string("Error in bind(): ") + strerror(errno));
    }

    // Start listening
    err = listen(sock.into_inner(), 128); // 128 is the backlog (max length of pending connections)
    if (err == -1) {
        throw Utils::runtime_error(std::string("Error in listen(): ") + strerror(errno));
    }
    return TcpListener(sock);
}

Socket const& TcpListener::socket(void) const { return inner; }

// Would need to be capable of returning SocketAddr instead of SocketAddrV4
// for Ipv6 functionality
Utils::pair<TcpStream, SocketAddrV4> TcpListener::accept(void) const {
    sockaddr_storage storage;
    socklen_t len = 0;

    Utils::memset(reinterpret_cast<void*>(&storage), 0, sizeof(storage));
    Socket sock = inner.accept(&storage, &len);
    SocketAddrV4 addr = SocketAddrV4::init(*reinterpret_cast<sockaddr_in*>(&storage));
    return Utils::make_pair(TcpStream(sock), addr);
}
