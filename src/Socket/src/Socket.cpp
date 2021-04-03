/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 10:05:07 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/03 21:50:39 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "../../Utils/src/Utils.hpp"
#include <cerrno>
#include <cstring>

Socket::Socket(void) {}

// inner FileDesc has it's own destructor, which calls close()
Socket::~Socket(void) {}

Socket::Socket(int fd) { inner = FileDesc::init(fd); }

Socket::Socket(Socket const& other) { *this = other; }

// Utilises the "move-semantics" of FileDesc, so that two socket's cannot
// possibly contain the same file descriptor
Socket& Socket::operator=(Socket const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    inner = rhs.inner;
    return *this;
}

Socket::Result Socket::init(const char* str, int type) {
    // If Ipv6 is implemented, logic here should change to use SocketAddr
    SocketAddrV4::Result addr = SocketAddrV4::init(str);

    if (addr.is_err())
        return Socket::Result::Err(addr.unwrap_err());

    return Socket::init(addr.unwrap(), type);
}

Socket::Result Socket::init(SocketAddr const& addr, int type) {
    int fd = 0;
#ifdef __APPLE__
    fd = socket(family, type, 0);
    if (fd == -1) {
        return Socket::Result::Err(strerror(errno));
    }
    // We'd maybe like to set CLOEXEC here for MacOs also, but ioctl is not permitted in
    // this project
    if (setsockopt(*this, SOL_SOCKET, SO_NOSIGPIPE, 1) == -1)
        return Socket::Result::Err(strerror(errno));
#elif __linux__
    fd = socket(addr.family(), type | SOCK_CLOEXEC, 0);
    if (fd == -1) {
        return Socket::Result::Err(strerror(errno));
    }
#endif
    return Socket::Result::Ok(Socket(fd));
}

int Socket::into_inner(void) const { return inner.raw(); }

bool Socket::connect(const struct sockaddr* addrp, socklen_t len) {
    int ret;

    ret = ::connect(inner.raw(), addrp, len);
    return (ret == 0) ? true : false;
}

Socket::Result Socket::accept(struct sockaddr* storage, socklen_t* len) const {
    Socket client_socket;
    int fd = -1;
    fd = ::accept(inner.raw(), storage, len);
    if (fd == -1) {
        return Socket::Result::Err(strerror(errno));
    }
    client_socket.inner = FileDesc::init(fd);
    return Socket::Result::Ok(client_socket);
}

Socket::Result Socket::accept(struct sockaddr_storage* storage, socklen_t* len) const {
    return accept(reinterpret_cast<sockaddr*>(storage), len);
}

Utils::RwResult Socket::recv_with_flags(void* buf, size_t len, int flags) {
    ssize_t ret;

    ret = recv(inner.raw(), buf, len, flags);
    if (ret == -1) {
        return Utils::RwResult::Err(strerror(errno));
    }
    return Utils::RwResult::Ok(ret);
}

Utils::RwResult Socket::read(void* buf, size_t len) { return recv_with_flags(buf, len, 0); }

Utils::RwResult Socket::peek(void* buf, size_t len) { return recv_with_flags(buf, len, MSG_PEEK); }

Utils::RwResult Socket::send_with_flags(const void* buf, size_t len, int flags) {
    ssize_t ret;

    ret = ::send(inner.raw(), buf, len, flags);
    if (ret == -1) {
        return Utils::RwResult::Err(strerror(errno));
    }
    return Utils::RwResult::Ok(ret);
}

Utils::RwResult Socket::send(const void* buf, size_t len) { return send_with_flags(buf, len, 0); }

bool Socket::operator==(Socket const& rhs) const { return inner.raw() == rhs.inner.raw(); }

bool Socket::operator!=(Socket const& rhs) const { return !(*this == rhs); }
