/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpListener.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 22:00:56 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/05 01:15:34 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "TcpListener.hpp"
#include "../../Utils/src/Utils.hpp"
#include <cerrno>
#include <cstring>

TcpListener::TcpListener(void) : inner(), config() { FD_ZERO(&config.current_sockets); }

TcpListener::~TcpListener(void) {}

TcpListener::TcpListener(Socket sock) : inner(sock), config() {
    FD_ZERO(&config.current_sockets);
    FD_SET(inner.into_inner(), &config.current_sockets);
    config.max_fd = inner.into_inner();
}

TcpListener::TcpListener(TcpListener const& other) { *this = other; }

TcpListener& TcpListener::operator=(TcpListener const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    inner = rhs.inner; // Move semantics preserved from FileDesc
    config = rhs.config;
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

    if (sock_res.is_err()) {
        return TcpListener::Result::Err(sock_res.unwrap_err());
    }
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

Utils::result<int, std::string> TcpListener::accept_raw(void) const {
    sockaddr_storage storage;
    socklen_t len = 0;

    Utils::memset(reinterpret_cast<void*>(&storage), 0, sizeof(storage));
    int fd = ::accept(inner.into_inner(), reinterpret_cast<sockaddr*>(&storage), &len);

    if (fd == -1) {
        return Utils::result<int, std::string>::Err(strerror(errno));
    }
    return Utils::result<int, std::string>::Ok(fd);
}

TcpListener::SelectResult TcpListener::select(void) {
    std::vector<TcpStream> clients;

    // ::select() is destructive - so a copy of current_sockets is required
    fd_set ready_sockets = config.current_sockets;

    if (::select(config.max_fd + 1, &ready_sockets, 0, 0, 0) < 0) {
        return TcpListener::SelectResult::Err(strerror(errno));
    }
    for (int fd = 0; fd < config.max_fd + 1; fd++) {
        if (FD_ISSET(fd, &ready_sockets)) {
            if (fd == inner.into_inner()) { // If fd == server socket -> new connection
                accept_new_client();
            } else { // If fd != server socket -> Ready to read from client
                Socket client = Socket::init_from_raw(fd);
                clients.push_back(TcpStream(client));

                FD_CLR(fd, &config.current_sockets);
            }
        }
    }
    return TcpListener::SelectResult::Ok(clients);
}

void TcpListener::accept_new_client(void) {
    Utils::result<int, std::string> res = this->accept_raw();

    if (res.is_ok()) {
        int raw_client_fd = res.unwrap();
        if (raw_client_fd > config.max_fd) {
            config.max_fd = raw_client_fd;
        }
        FD_SET(raw_client_fd, &config.current_sockets);
    } else {
        // print error to stderr and continue (No need to crash entire server)
        std::cerr << "accept_raw() Error: " << res.unwrap_err() << std::endl;
    }
}

bool TcpListener::operator==(TcpListener const& other) const { return inner == other.inner; }

bool TcpListener::operator!=(TcpListener const& other) const { return !(*this == other); }

// SelectConfig is a simple data structure for TcpListener::select calls
TcpListener::SelectConfig::SelectConfig(void) : current_sockets(), max_fd(0) {}

TcpListener::SelectConfig::~SelectConfig(void) {}
