/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpListener.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 21:57:46 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/03 15:21:11 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPLISTENER_HPP
#define TCPLISTENER_HPP

#include "../../Utils/src/pair.hpp"
#include "Socket.hpp"
#include "TcpStream.hpp"

class TcpListener {
  public:
    typedef Utils::result<TcpListener, std::string> Result;
    typedef Utils::result<Utils::pair<TcpStream, SocketAddrV4>, std::string> AcceptResult;

  public:
    TcpListener(void);
    ~TcpListener(void);
    TcpListener(Socket sock);
    TcpListener(TcpListener const& other);
    TcpListener& operator=(TcpListener const& rhs);

    Result bind(const char* str);
    Result bind(SocketAddr const& addr);

    Socket const& socket(void) const;

    AcceptResult accept(void) const;

  private:
    Socket inner;
};

#endif
