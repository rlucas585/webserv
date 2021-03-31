/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpListener.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 21:57:46 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/28 22:41:04 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPLISTENER_HPP
#define TCPLISTENER_HPP

#include "../../Utils/src/pair.hpp"
#include "Socket.hpp"
#include "TcpStream.hpp"

class TcpListener {
  public:
    TcpListener(void);
    ~TcpListener(void);
    TcpListener(Socket sock);
    TcpListener(TcpListener const& other);
    TcpListener& operator=(TcpListener const& rhs);

    static TcpListener bind(const char* str);
    static TcpListener bind(SocketAddr const& addr);

    Socket const& socket(void) const;

    Utils::pair<TcpStream, SocketAddrV4> accept(void) const;

  private:
    Socket inner;
};

#endif
