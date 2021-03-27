/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 10:05:07 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/27 14:45:14 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "../../Utils/src/Utils.hpp"
#include <cerrno>
#include <cstring>

// inner FileDesc has it's own destructor, which calls close()
Socket::~Socket(void) {}

Socket::Socket(int family, int type) {
    int fd = 0;
    int yes = 1;
    int err = 0;
#ifdef __APPLE__
    fd = socket(family, type, 0);
    if (fd == -1) {
        throw Utils::runtime_error("Error in socket()");
    }
    inner = FileDesc::init(fd);
    // We'd maybe like to set CLOEXEC here for MacOs also, but ioctl is not permitted in
    // this project
    err = setsockopt(this->into_inner(), SOL_SOCKET, SO_REUSEADDR | SO_NOSIGPIPE, &yes, sizeof(yes));
    if (err == -1) {
        throw Utils::runtime_error(std::string("Error in setsockopt(): ") + strerror(errno));
    }
#elif __linux__
    fd = socket(family, type | SOCK_CLOEXEC, 0);
    if (fd == -1) {
        throw Utils::runtime_error(std::string("Error in socket(): ") + strerror(errno));
    }
    inner = FileDesc::init(fd);
    err = setsockopt(this->into_inner(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (err == -1) {
        throw Utils::runtime_error(std::string("Error in setsockopt(): ") + strerror(errno));
    }
#endif
}

Socket::Socket(Socket const& other) { *this = other; }

// Utilises the "move-semantics" of FileDesc, so that two socket's cannot
// possibly contain the same file descriptor
Socket &Socket::operator=(Socket const& rhs) {
    if (this == &rhs) { return *this; }
    inner = rhs.inner;
    return *this;
}

Socket  Socket::init(const char* str, int type) {
    // If Ipv6 is implemented, logic here should change to use SocketAddr
    SocketAddrV4  addr = SocketAddrV4::init(str);

    return Socket::init(addr, type);
}

Socket  Socket::init(SocketAddr const& addr, int type) {
    return Socket(addr.family(), type);
}

int Socket::into_inner(void) const {
    return inner.raw();
}
