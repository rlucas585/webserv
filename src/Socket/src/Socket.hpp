/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 09:55:51 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/27 16:27:39 by rlucas        ########   odam.nl         */
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
    Socket accept(struct sockaddr* storage, socklen_t* len);
    ssize_t recv_with_flags(void* buf, size_t len, int flags);
    ssize_t read(void* buf, size_t len);
    ssize_t peek(void* buf, size_t len);

  private:
    FileDesc inner;

    // Socket(SocketAddrV4 const& addr, int type);
    Socket(int family, int type);
};

#endif
