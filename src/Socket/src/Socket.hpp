/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 09:55:51 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/03 21:49:26 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../../FileDesc/src/FileDesc.hpp"
#include "../../Result/src/result.hpp"
#include "SocketAddr.hpp"

class Socket {
  public:
    typedef Utils::result<Socket, std::string> Result;

  public:
    Socket(void);
    ~Socket(void);
    Socket(Socket const& other);
    Socket& operator=(Socket const& rhs);

    static Result init(const char* str, int type);
    static Result init(SocketAddr const& addr, int type);

    template <typename T> static int setsockopt(Socket const& sock, int options, int value, T payload) {
        void* payload_ptr = reinterpret_cast<void*>(&payload);
        return ::setsockopt(sock.into_inner(), options, value, payload_ptr, static_cast<socklen_t>(sizeof(T)));
    }

    int into_inner(void) const;
    bool connect(const struct sockaddr* addrp, socklen_t len);
    Result accept(struct sockaddr* storage, socklen_t* len) const;
    Result accept(struct sockaddr_storage* storage, socklen_t* len) const;
    Utils::RwResult recv_with_flags(void* buf, size_t len, int flags);
    Utils::RwResult read(void* buf, size_t len);
    Utils::RwResult peek(void* buf, size_t len);
    Utils::RwResult send_with_flags(const void* buf, size_t len, int flags);
    Utils::RwResult send(const void* buf, size_t len);

    bool operator==(Socket const& rhs) const;
    bool operator!=(Socket const& rhs) const;

  private:
    FileDesc inner;

    Socket(int fd);
};

#endif
