/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   SocketAddr.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/24 18:59:55 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/24 19:08:40 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "SocketAddr.hpp"

SocketAddrV4::SocketAddrV4(void) {
}

SocketAddrV4::SocketAddrV4(Ipv4Addr ip, u_int16_t port) {
    inner.sin_family = AF_INET;
}
