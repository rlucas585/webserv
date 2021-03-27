/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 09:55:51 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/27 22:20:06 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../../FileDesc/src/FileDesc.hpp"
#include "SocketAddr.hpp"

class Socket {
  public:
    Socket(void);
    ~Socket(void);
    Socket(Socket const& other);
    Socket& operator=(Socket const& rhs);

    static Socket init(const char* str, int type);
    static Socket init(SocketAddr const& addr, int type);

    int into_inner(void) const;
    bool connect(const struct sockaddr* addrp, socklen_t len);
    Socket accept(struct sockaddr* storage, socklen_t* len);
    ssize_t recv_with_flags(void* buf, size_t len, int flags);
    ssize_t read(void* buf, size_t len);
    ssize_t peek(void* buf, size_t len);
    ssize_t send_with_flags(const void* buf, size_t len, int flags);
    ssize_t send(const void* buf, size_t len);

  private:
    FileDesc inner;

    // Socket(SocketAddrV4 const& addr, int type);
    Socket(int family, int type);
};

#endif
