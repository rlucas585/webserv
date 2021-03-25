/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IpAddr.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/24 18:38:46 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/25 10:48:43 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPADDR_HPP
#define IPADDR_HPP

#include <netinet/in.h>

class IpAddr {
    public:
        static u_int32_t    convert_to_big_endian(u_int8_t bytes[4]);
        static u_int32_t    convert_to_little_endian(u_int8_t bytes[4]);
        static u_int32_t    network_to_host_long(u_int32_t netlong);
        static u_int16_t    network_to_host_short(u_int16_t netshort);
        static u_int32_t    host_to_network_long(u_int32_t hostlong);
        static u_int16_t    host_to_network_short(u_int16_t hostshort);

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

#endif
