/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpStream.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 22:32:29 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/31 14:23:29 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "TcpStream.hpp"
#include "../../Utils/src/Utils.hpp"
#include <cerrno>
#include <cstring>

TcpStream::TcpStream(void) : inner() {}

TcpStream::~TcpStream(void) {}

TcpStream::TcpStream(Socket sock) : inner(sock) {}

TcpStream::TcpStream(TcpStream const& other) {
    *this = other;
}

// Requires updating for Ipv6
TcpStream TcpStream::connect(const char* str) {
    return TcpStream::connect(SocketAddrV4::init(str));
}

TcpStream TcpStream::connect(SocketAddr const& addr) {
    Utils::pair<const sockaddr*, socklen_t> addr_info = addr.into_inner();
    const sockaddr* addrp = addr_info.first;
    socklen_t len = addr_info.second;
    int err = 0;

    Socket sock = Socket::init(addr, SOCK_STREAM);

    err = ::connect(sock.into_inner(), addrp, len);
    if (err == -1) {
        throw Utils::runtime_error(std::string("Error in connect(): ") + strerror(errno));
    }
    return TcpStream(sock);
}

ssize_t TcpStream::read(void *buf, size_t len) {
    return inner.read(buf, len);
}

ssize_t TcpStream::write(const void *buf, size_t len) {
    return inner.send(buf, len);
}

ssize_t TcpStream::write(const char *str) {
    return inner.send(reinterpret_cast<const void*>(str), Utils::strlen(str));
}

ssize_t TcpStream::write(std::string const& str) {
    return inner.send(reinterpret_cast<const void*>(str.c_str()), str.size());
}

TcpStream &TcpStream::operator=(TcpStream const& rhs) {
    if (this == &rhs) { return *this; }
    inner = rhs.inner;  // Move semantics preserved from FileDesc
    return *this;
}
