/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpStream.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 22:27:42 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/31 14:06:46 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSTREAM_HPP
#define TCPSTREAM_HPP

#include "Socket.hpp"

class TcpStream {
    public:
        ~TcpStream(void);
        TcpStream(Socket sock);
        TcpStream(TcpStream const& other);
        TcpStream &operator=(TcpStream const& rhs);

        static TcpStream connect(const char* str);
        static TcpStream connect(SocketAddr const& addr);

        ssize_t read(void *buf, size_t len);
        ssize_t write(const void *buf, size_t len);
        ssize_t write(const char *str);
        ssize_t write(std::string const& str);

    private:
        Socket inner;
        TcpStream(void);
};

#endif
