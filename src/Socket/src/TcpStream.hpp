/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpStream.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 22:27:42 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/05 10:41:09 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSTREAM_HPP
#define TCPSTREAM_HPP

#include "../../Result/src/result.hpp"
#include "Socket.hpp"

class TcpStream {
  public:
    typedef Utils::result<TcpStream, std::string> Result;

  public:
    ~TcpStream(void);
    TcpStream(Socket sock);
    TcpStream(TcpStream const& other);
    TcpStream& operator=(TcpStream const& rhs);

    static TcpStream::Result connect(const char* str);
    static TcpStream::Result connect(SocketAddr const& addr);

    Utils::RwResult read(void* buf, size_t len);
    Utils::RwResult read(std::string& str);
    Utils::RwResult write(const void* buf, size_t len);
    Utils::RwResult write(const char* str);
    Utils::RwResult write(std::string const& str);

  private:
    Socket inner;
    TcpStream(void);
};

#endif
