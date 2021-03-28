/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpStream.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 22:27:42 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/28 22:32:15 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSTREAM_HPP
#define TCPSTREAM_HPP

#include "Socket.hpp"

class TcpStream {
        TcpStream(void);
        ~TcpStream(void);
        TcpStream(Socket sock);
        TcpStream(TcpStream const& other);
        TcpStream &operator=(TcpStream const& rhs);

    private:
        Socket inner;
};

#endif
