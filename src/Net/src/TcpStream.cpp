/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpStream.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 22:32:29 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/05 11:26:54 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "TcpStream.hpp"
#include "../../Utils/src/Utils.hpp"
#include <cerrno>
#include <cstring>
#include <vector>

TcpStream::TcpStream(void) : inner() {}

TcpStream::~TcpStream(void) {}

TcpStream::TcpStream(Socket sock) : inner(sock) {}

TcpStream::TcpStream(TcpStream const& other) { *this = other; }

// Requires updating for Ipv6
TcpStream::Result TcpStream::connect(const char* str) {
    SocketAddrV4::Result addr = SocketAddrV4::init(str);

    if (addr.is_err()) {
        return TcpStream::Result::Err(addr.unwrap_err());
    }
    return TcpStream::connect(addr.unwrap());
}

TcpStream::Result TcpStream::connect(SocketAddr const& addr) {
    Utils::pair<const sockaddr*, socklen_t> addr_info = addr.into_inner();
    const sockaddr* addrp = addr_info.first;
    socklen_t len = addr_info.second;

    Socket::Result sock_res = Socket::init(addr, SOCK_STREAM);

    if (sock_res.is_err()) {
        return TcpStream::Result::Err(sock_res.unwrap_err());
    }
    Socket sock = sock_res.unwrap();

    if (::connect(sock.into_inner(), addrp, len) == -1) {
        return TcpStream::Result::Err(strerror(errno));
    }
    return TcpStream(sock);
}

Utils::RwResult TcpStream::read(void* buf, size_t len) { return inner.read(buf, len); }

// I'm not positive on the most efficient way to Read into a string from a
// socket - this utilizes a char vector, removes '\0', then appends it to a
// string, but maybe there are better ways.
Utils::RwResult TcpStream::read(std::string& str) {
    std::vector<char> buffer(Socket::READ_LIMIT, 0);
    Utils::RwResult ret = inner.read(&buffer[0], Socket::READ_LIMIT);

    if (ret.is_err()) {
        return ret;
    }
    for (std::vector<char>::reverse_iterator it = buffer.rbegin();
         it != buffer.rend() && *it == 0;) {
        std::vector<char>::reverse_iterator tmp = it;
        tmp++;
        buffer.erase(--(it.base()));
        it = tmp;
    }

    str.append(buffer.begin(), buffer.end());

    return ret;
}

Utils::RwResult TcpStream::write(const void* buf, size_t len) { return inner.send(buf, len); }

Utils::RwResult TcpStream::write(const char* str) {
    return inner.send(reinterpret_cast<const void*>(str), Utils::strlen(str));
}

Utils::RwResult TcpStream::write(std::string const& str) {
    return inner.send(reinterpret_cast<const void*>(str.c_str()), str.size());
}

TcpStream& TcpStream::operator=(TcpStream const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    inner = rhs.inner; // Move semantics preserved from FileDesc
    return *this;
}