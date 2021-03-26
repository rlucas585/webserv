/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   SocketAddr.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/24 18:57:14 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/24 19:02:05 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETADDR_HPP
#define SOCKETADDR_HPP

#include "IpAddr.hpp"
#include <arpa/inet.h>

class SocketAddr {};

class SocketAddrV4 : public SocketAddr {
  public:
    static SocketAddrV4 init(Ipv4Addr ip, u_int16_t port);

  private:
    sockaddr_in inner;

    SocketAddrV4(void);
    SocketAddrV4(Ipv4Addr ip, u_int16_t port);
};

#endif
