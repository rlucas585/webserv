/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpStream.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 22:32:29 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/28 22:33:44 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "TcpStream.hpp"

TcpStream::TcpStream(void) : inner() {}

TcpStream::~TcpStream(void) {}

TcpStream::TcpStream(Socket sock) : inner(sock) {}

TcpStream::TcpStream(TcpStream const& other) {
    *this = other;
}

TcpStream &TcpStream::operator=(TcpStream const& rhs) {
    if (this == &rhs) { return *this; }
    inner = rhs.inner;  // Move semantics preserved from FileDesc
    return *this;
}
