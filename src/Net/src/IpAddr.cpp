/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IpAddr.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/24 18:39:05 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/02 23:10:32 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "IpAddr.hpp"
#include "../../Utils/src/Utils.hpp"
#include <arpa/inet.h>
#include <string>

u_int32_t IpAddr::u32_convert_to_big_endian(u_int8_t bytes[4]) {
    u_int32_t output = 0;

    output = output | bytes[0];
    output = output | bytes[1] << 8;
    output = output | bytes[2] << 16;
    output = output | bytes[3] << 24;
    return output;
}

u_int32_t IpAddr::u32_convert_to_little_endian(u_int8_t bytes[4]) {
    u_int32_t output = 0;

    output = output | bytes[0] << 24;
    output = output | bytes[1] << 16;
    output = output | bytes[2] << 8;
    output = output | bytes[3];
    return output;
}

u_int16_t IpAddr::u16_convert_to_big_endian(u_int8_t bytes[2]) {
    u_int16_t output = 0;

    output = output | bytes[0];
    output = output | bytes[1] << 8;
    return output;
}

u_int16_t IpAddr::u16_convert_to_little_endian(u_int8_t bytes[2]) {
    u_int16_t output = 0;

    output = output | bytes[0] << 8;
    output = output | bytes[1];
    return output;
}

u_int32_t IpAddr::network_to_host_long(u_int32_t netlong) {
    u_int8_t bytes[4];

    if (host_is_big_endian()) {
        return netlong;
    }
    bytes[0] = netlong;
    bytes[1] = netlong >> 8;
    bytes[2] = netlong >> 16;
    bytes[3] = netlong >> 24;
    return u32_convert_to_little_endian(bytes);
}

u_int16_t IpAddr::network_to_host_short(u_int16_t netshort) {
    u_int8_t bytes[2];

    if (host_is_big_endian()) {
        return netshort;
    }
    bytes[0] = netshort;
    bytes[1] = netshort >> 8;
    return u16_convert_to_little_endian(bytes);
}

u_int32_t IpAddr::host_to_network_long(u_int32_t hostlong) {
    u_int8_t bytes[4];

    if (host_is_big_endian()) {
        return hostlong;
    }
    bytes[0] = hostlong >> 24;
    bytes[1] = hostlong >> 16;
    bytes[2] = hostlong >> 8;
    bytes[3] = hostlong;
    return u32_convert_to_big_endian(bytes);
}

u_int16_t IpAddr::host_to_network_short(u_int16_t hostshort) {
    u_int8_t bytes[2];

    if (host_is_big_endian()) {
        return hostshort;
    }
    bytes[0] = hostshort >> 8;
    bytes[1] = hostshort;
    return u16_convert_to_big_endian(bytes);
}

bool IpAddr::host_is_little_endian(void) {
    static bool set = false;
    static bool status = false;

    if (set == false) {
        int n = 1;
        if (*reinterpret_cast<char*>(&n) == 1) {
            status = true;
        } else {
            status = false;
            return false;
        }
        set = true;
    }
    return status;
}

bool IpAddr::host_is_big_endian(void) { return !host_is_little_endian(); }

Ipv4Addr::Ipv4Addr(void) { inner.s_addr = 0; }

Ipv4Addr::~Ipv4Addr(void) {}

Ipv4Addr::Ipv4Addr(u_int8_t bytes[4]) { inner.s_addr = u32_convert_to_big_endian(bytes); }

Ipv4Addr::Ipv4Addr(struct in_addr address) { inner = address; }

Ipv4Addr::Ipv4Addr(Ipv4Addr const& other) { *this = other; }

Ipv4Addr& Ipv4Addr::operator=(Ipv4Addr const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    inner = rhs.inner;
    return *this;
}

Ipv4Addr Ipv4Addr::init_from_bytes(u_int8_t bytes[4]) { return Ipv4Addr(bytes); }

Ipv4Addr Ipv4Addr::init_from_bytes(u_int8_t a, u_int8_t b, u_int8_t c, u_int8_t d) {
    u_int8_t bytes[4] = {a, b, c, d};

    return Ipv4Addr(bytes);
}

Ipv4Addr::Result Ipv4Addr::init_from_string(const char* str) {
    return Ipv4Addr::init_from_string(Str(str));
}

Ipv4Addr::Result Ipv4Addr::init_from_string(Str const& ip_str) {
    std::string buf; // Used to isolate and null terminate the Str
    in_addr address;

    if (!ip_str.isInitialized() || ip_str.length() > 15) {
        return Ipv4Addr::Result::Err("Invalid Str used for Ipv4Addr");
    }
    if (ip_str == "localhost") {
        address.s_addr = inet_addr("127.0.0.1");
        return Ipv4Addr::Result::Ok(Ipv4Addr(address));
    }
    buf = ip_str.toString();
    buf.push_back(' ');
    address.s_addr = inet_addr(buf.c_str());
    if (address.s_addr == INADDR_NONE) {
        return Ipv4Addr::Result::Err("Invalid Str used for Ipv4Addr");
    }
    return Ipv4Addr::Result::Ok(Ipv4Addr(address));
}

// We'd like to return a [u8; 4], but this is not possible in C++.
// A std::vector is possible, but feels like overkill.
void Ipv4Addr::octets(u_int8_t bytes[4]) const {
    bytes[0] = inner.s_addr;
    bytes[1] = inner.s_addr >> 8;
    bytes[2] = inner.s_addr >> 16;
    bytes[3] = inner.s_addr >> 24;
}

bool Ipv4Addr::is_unspecified(void) const { return inner.s_addr == 0; }

bool Ipv4Addr::is_loopback(void) const {
    u_int8_t bytes[4];

    octets(bytes);
    return bytes[0] == 127 && bytes[1] == 0 && bytes[2] == 0 && bytes[3] == 1;
}

in_addr Ipv4Addr::into_inner(void) const { return inner; }

bool Ipv4Addr::operator==(Ipv4Addr const& other) const {
    return inner.s_addr == other.inner.s_addr;
}

bool Ipv4Addr::operator!=(Ipv4Addr const& other) const { return !(*this == other); }

bool Ipv4Addr::operator==(in_addr const& other) const { return inner.s_addr == other.s_addr; }

bool Ipv4Addr::operator!=(in_addr const& other) const { return !(*this == other); }

bool operator==(in_addr const& lhs, Ipv4Addr const& rhs) { return rhs == lhs; }

bool operator!=(in_addr const& lhs, Ipv4Addr const& rhs) { return rhs != lhs; }
