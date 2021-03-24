/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   SocketAddr.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/20 19:33:34 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/24 17:45:35 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETADDR_HPP
#define SOCKETADDR_HPP

#include <netinet/in.h>

class IpAddr {
    public:
        static u_int32_t    convert_to_big_endian(u_int8_t bytes[4]);
        static u_int32_t    convert_to_little_endian(u_int8_t bytes[4]);
        static u_int32_t    network_to_host_long(u_int32_t netlong);

        static bool         host_is_little_endian(void);
        static bool         host_is_big_endian(void);
};

class Ipv4Addr : public IpAddr {
    public:
        Ipv4Addr(void);
        ~Ipv4Addr(void);
        Ipv4Addr(Ipv4Addr const& other);
        Ipv4Addr&   operator=(Ipv4Addr const& rhs);

        static Ipv4Addr     init_from_bytes(u_int8_t bytes[4]);
        static Ipv4Addr     init_from_bytes(u_int8_t a, u_int8_t b, u_int8_t c, u_int8_t d);
        static Ipv4Addr     init_from_string(const char* str);

        void                octets(u_int8_t bytes[4]) const;
        bool                is_unspecified(void) const;
        bool                is_loopback(void) const;

        bool    operator==(Ipv4Addr const& other) const;
        bool    operator!=(Ipv4Addr const& other) const;

    private:
        in_addr         inner;

        Ipv4Addr(u_int8_t bytes[4]);
        Ipv4Addr(const char* str);
};

class SocketAddr {

};

class SocketAddrV4 : public SocketAddr {

    private:
        sockaddr_in     inner;
};

#endif
