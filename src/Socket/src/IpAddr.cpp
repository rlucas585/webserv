/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IpAddr.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/24 18:39:05 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/26 11:00:52 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "IpAddr.hpp"
#include <arpa/inet.h>
#include "../../Utils/src/Utils.hpp"

u_int32_t       IpAddr::u32_convert_to_big_endian(u_int8_t bytes[4]) {
    u_int32_t       output = 0;

    output = output | bytes[0];
    output = output | bytes[1] << 8;
    output = output | bytes[2] << 16;
    output = output | bytes[3] << 24;
    return output;
}

u_int32_t       IpAddr::u32_convert_to_little_endian(u_int8_t bytes[4]) {
    u_int32_t       output = 0;

    output = output | bytes[0] << 24;
    output = output | bytes[1] << 16;
    output = output | bytes[2] << 8;
    output = output | bytes[3];
    return output;
}

u_int16_t    IpAddr::u16_convert_to_big_endian(u_int8_t bytes[2]) {
    u_int16_t       output = 0;

    output = output | bytes[0];
    output = output | bytes[1] << 8;
    return output;
}

u_int16_t    IpAddr::u16_convert_to_little_endian(u_int8_t bytes[2]) {
    u_int16_t       output = 0;

    output = output | bytes[0] << 8;
    output = output | bytes[1];
    return output;
}

u_int32_t       IpAddr::ntohl(u_int32_t netlong) {
    u_int8_t              bytes[4];

    if (host_is_big_endian()) {
        return netlong;
    }
    bytes[0] = netlong;
    bytes[1] = netlong >> 8;
    bytes[2] = netlong >> 16;
    bytes[3] = netlong >> 24;
    return u32_convert_to_little_endian(bytes);
}

u_int16_t       IpAddr::ntohs(u_int16_t netshort) {
    u_int8_t            bytes[2];

    if (host_is_big_endian()) {
        return netshort;
    }
    bytes[0] = netshort;
    bytes[1] = netshort >> 8;
    return u16_convert_to_little_endian(bytes);
}

u_int32_t       IpAddr::htonl(u_int32_t hostlong) {
    u_int8_t            bytes[4];

    if (host_is_big_endian()) {
        return hostlong;
    }
    bytes[0] = hostlong >> 24;
    bytes[1] = hostlong >> 16;
    bytes[2] = hostlong >> 8;
    bytes[3] = hostlong;
    return u32_convert_to_big_endian(bytes);
}

u_int16_t       IpAddr::htons(u_int16_t hostshort) {
    u_int8_t            bytes[2];

    if (host_is_big_endian()) {
        return hostshort;
    }
    bytes[0] = hostshort >> 8;
    bytes[1] = hostshort;
    return u16_convert_to_big_endian(bytes);
}

bool            IpAddr::host_is_little_endian(void) {
    static bool set = false;
    static bool status = false;

    if (set == false) {
        int     n = 1;
        if (*reinterpret_cast<char *>(&n) == 1) {
            status = true;
        } else {
            status = false;
            return false;
        }
        set = true;
    }
    return status;
}

bool            IpAddr::host_is_big_endian(void) {
    return !host_is_little_endian();
}

Ipv4Addr::Ipv4Addr(void) {
    inner.s_addr = 0;
}

Ipv4Addr::~Ipv4Addr(void) {
}

Ipv4Addr::Ipv4Addr(u_int8_t bytes[4]) {
    inner.s_addr = u32_convert_to_big_endian(bytes);
}

Ipv4Addr::Ipv4Addr(const char* str) {
    inner.s_addr = inet_addr(str);
    if (inner.s_addr == INADDR_NONE) {
        throw Utils::runtime_error("Invalid string used for Ipv4Addr");
    }
}

Ipv4Addr::Ipv4Addr(Ipv4Addr const& other) {
    *this = other;
}

Ipv4Addr    &Ipv4Addr::operator=(Ipv4Addr const& rhs) {
    if (this == &rhs) { return *this; }
    inner = rhs.inner;
    return *this;
}

Ipv4Addr    Ipv4Addr::init_from_bytes(u_int8_t bytes[4]) {
    return Ipv4Addr(bytes);
}

Ipv4Addr    Ipv4Addr::init_from_bytes(u_int8_t a, u_int8_t b, u_int8_t c, u_int8_t d) {
    u_int8_t      bytes[4] = {a, b, c, d};

    return Ipv4Addr(bytes);
}

Ipv4Addr    Ipv4Addr::init_from_string(const char *str) {
    return Ipv4Addr(str);
}

// We'd like to return a [u8; 4], but this is not possible in C++.
// A std::vector is possible, but feels like overkill.
void        Ipv4Addr::octets(u_int8_t bytes[4]) const {
    bytes[0] = inner.s_addr;
    bytes[1] = inner.s_addr >> 8;
    bytes[2] = inner.s_addr >> 16;
    bytes[3] = inner.s_addr >> 24;
}

bool        Ipv4Addr::is_unspecified(void) const {
    return inner.s_addr == 0;
}

bool        Ipv4Addr::is_loopback(void) const {
    u_int8_t        bytes[4];

    octets(bytes);
    return bytes[0] == 127;
}

bool        Ipv4Addr::operator==(Ipv4Addr const& other) const {
    return inner.s_addr == other.inner.s_addr;
}

bool        Ipv4Addr::operator!=(Ipv4Addr const& other) const {
    return !(*this == other);
}
