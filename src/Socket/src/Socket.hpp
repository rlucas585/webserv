/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 09:55:51 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/27 13:38:59 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../../FileDesc/src/FileDesc.hpp"
#include "SocketAddr.hpp"

class Socket {
    public:
        ~Socket(void);
        Socket(Socket const& other);
        Socket &operator=(Socket const& rhs);

        static Socket   init(const char* str, int type);
        static Socket   init(SocketAddr const& addr, int type);

        int into_inner(void) const;

    private:
        FileDesc    inner;

        Socket(void);    // Left undefined
        // Socket(SocketAddrV4 const& addr, int type);
        Socket(int family, int type);
};

#endif
