/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   SocketAddr.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/24 18:57:14 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/03 14:17:35 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETADDR_HPP
#define SOCKETADDR_HPP

#include "../../Result/src/result.hpp"
#include "../../Str/src/Str.hpp"
#include "../../Utils/src/pair.hpp"
#include "IpAddr.hpp"
#include <arpa/inet.h>

class SocketAddr {
  public:
    virtual int family(void) const = 0;
    // For Ipv6, should be changed to sockaddr_storage*
    virtual Utils::pair<const sockaddr*, socklen_t> into_inner(void) const = 0;
};

class SocketAddrV4 : public SocketAddr {
  public:
    typedef Utils::result<SocketAddrV4, std::string> Result;

  public:
    ~SocketAddrV4(void);
    SocketAddrV4(SocketAddrV4 const& other);
    SocketAddrV4 operator=(SocketAddrV4 const& rhs);

    static SocketAddrV4 init(Ipv4Addr ip, u_int16_t port);
    static Result init(Str const& socket_addr_str);
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

// Overloads for sockaddr_in also created
bool operator==(sockaddr_in const& lhs, SocketAddrV4 const& rhs);
bool operator!=(sockaddr_in const& lhs, SocketAddrV4 const& rhs);
bool operator==(sockaddr_in const& lhs, sockaddr_in const& rhs);
bool operator!=(sockaddr_in const& lhs, sockaddr_in const& rhs);

#endif
