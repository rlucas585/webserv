/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   SocketAddr.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/24 18:57:14 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/31 09:30:52 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETADDR_HPP
#define SOCKETADDR_HPP

#include "../../Str/src/Str.hpp"
#include "../../Utils/src/pair.hpp"
#include "IpAddr.hpp"
#include <arpa/inet.h>

class SocketAddr {
  public:
    virtual int family(void) const = 0;
    virtual Utils::pair<const sockaddr*, socklen_t> into_inner(void) const = 0;
};

class SocketAddrV4 : public SocketAddr {
  public:
    ~SocketAddrV4(void);
    SocketAddrV4(SocketAddrV4 const& other);
    SocketAddrV4 operator=(SocketAddrV4 const& rhs);

    static SocketAddrV4 init(Ipv4Addr ip, u_int16_t port);
    static SocketAddrV4 init(Str const& socket_addr_str);
    static SocketAddrV4 init(sockaddr_in const& storage);

    Ipv4Addr const& ip(void) const;
    void set_ip(Ipv4Addr new_ip);
    u_int16_t port(void) const;
    void set_port(u_int16_t new_port);

    Utils::pair<const sockaddr*, socklen_t> into_inner(void) const;
    int family(void) const;

    bool operator==(SocketAddrV4 const& other) const;
    bool operator!=(SocketAddrV4 const& other) const;
    bool operator==(sockaddr_in const& other) const;
    bool operator!=(sockaddr_in const& other) const;

  private:
    sockaddr_in inner;

    SocketAddrV4(void);
    SocketAddrV4(Ipv4Addr ip, u_int16_t port);
    SocketAddrV4(sockaddr_in const& storage);
};

bool operator==(sockaddr_in const& lhs, SocketAddrV4 const& rhs);
bool operator!=(sockaddr_in const& lhs, SocketAddrV4 const& rhs);
bool operator==(sockaddr_in const& lhs, sockaddr_in const& rhs);
bool operator!=(sockaddr_in const& lhs, sockaddr_in const& rhs);

#endif
