#ifndef IPADDR_HPP
#define IPADDR_HPP

#include "result.hpp"
#include "Slice.hpp"
#include <netinet/in.h>

class IpAddr {
  public:
    static u_int32_t u32_convert_to_big_endian(u_int8_t bytes[4]);
    static u_int32_t u32_convert_to_little_endian(u_int8_t bytes[4]);
    static u_int16_t u16_convert_to_big_endian(u_int8_t bytes[2]);
    static u_int16_t u16_convert_to_little_endian(u_int8_t bytes[2]);
    static u_int32_t network_to_host_long(u_int32_t netlong);
    static u_int16_t network_to_host_short(u_int16_t netshort);
    static u_int32_t host_to_network_long(u_int32_t hostlong);
    static u_int16_t host_to_network_short(u_int16_t hostshort);
    static std::string network_to_presentation(const void* src);

    static bool host_is_little_endian(void);
    static bool host_is_big_endian(void);
};

class Ipv4Addr : public IpAddr {
  public:
    typedef Utils::result<Ipv4Addr, std::string> Result;

  public:
    Ipv4Addr(void);
    ~Ipv4Addr(void);
    Ipv4Addr(Ipv4Addr const& other);
    Ipv4Addr& operator=(Ipv4Addr const& rhs);

    static Ipv4Addr init_from_bytes(u_int8_t bytes[4]);
    static Ipv4Addr init_from_bytes(u_int8_t a, u_int8_t b, u_int8_t c, u_int8_t d);
    static Result init_from_string(const char* str);
    static Result init_from_string(Slice const& str);

    void octets(u_int8_t bytes[4]) const;
    bool is_unspecified(void) const;
    bool is_loopback(void) const;

    in_addr into_inner(void) const;

    bool operator==(Ipv4Addr const& other) const;
    bool operator!=(Ipv4Addr const& other) const;
    bool operator==(in_addr const& other) const;
    bool operator!=(in_addr const& other) const;

  private:
    in_addr inner;

    Ipv4Addr(u_int8_t bytes[4]);
    Ipv4Addr(struct in_addr address);
};

bool operator==(in_addr const& lhs, Ipv4Addr const& rhs);
bool operator!=(in_addr const& lhs, Ipv4Addr const& rhs);

#endif
