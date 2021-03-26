/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   SocketAddr.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/26 13:50:51 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/26 18:21:38 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/SocketAddr.hpp"
#include <gtest/gtest.h>

TEST(SocketAddr_tests, creation_test) {
    const char* ip_str = "127.2.1.1";
    const u_int16_t port = 1500;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str);
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);
    struct sockaddr_in expected_addr;

    expected_addr.sin_family = AF_INET;
    expected_addr.sin_port = htons(port);
    expected_addr.sin_addr.s_addr = inet_addr(ip_str);
    memset(expected_addr.sin_zero, 0, 8);

    EXPECT_EQ(socket_addr, expected_addr);
}

TEST(SocketAddr_tests, ip_test) {
    const char* ip_str = "127.2.1.1";
    const u_int16_t port = 1500;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str);
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);

    in_addr expected_ip;
    expected_ip.s_addr = inet_addr(ip_str);

    Ipv4Addr const& addr_ref = socket_addr.ip();

    EXPECT_EQ(addr_ref, addr);
    EXPECT_EQ(addr_ref, expected_ip);
}

TEST(SocketAddr_tests, port_test) {
    const char* ip_str = "127.2.1.1";
    const u_int16_t port = 1500;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str);
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);

    EXPECT_EQ(socket_addr.port(), port);
}

TEST(SocketAddr_tests, set_ip_test) {
    const char* ip_str = "127.2.1.1";
    const char* ip_str2 = "204.15.172.14";
    const u_int16_t port = 1500;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str);
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);

    in_addr expected_ip;
    expected_ip.s_addr = inet_addr(ip_str);

    EXPECT_EQ(socket_addr.ip(), expected_ip);

    // Set new Ip Address (with same port)
    socket_addr.set_ip(Ipv4Addr::init_from_string(ip_str2));
    expected_ip.s_addr = inet_addr(ip_str2);

    EXPECT_EQ(socket_addr.ip(), expected_ip);
}

TEST(SocketAddr_tests, set_port_test) {
    const char* ip_str = "127.2.1.1";
    const u_int16_t port = 1500;
    const u_int16_t port2 = 4242;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str);
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);

    EXPECT_EQ(socket_addr.port(), port);

    // Change port (with same Ip address)
    socket_addr.set_port(port2);

    EXPECT_EQ(socket_addr.port(), port2);
}

TEST(SocketAddr_tests, into_inner_test) {
    const char* ip_str = "127.2.1.1";
    const u_int16_t port = 1500;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str);
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);
    struct sockaddr_in expected_addr;

    expected_addr.sin_family = AF_INET;
    expected_addr.sin_port = htons(port);
    expected_addr.sin_addr.s_addr = inet_addr(ip_str);
    memset(expected_addr.sin_zero, 0, 8);

    EXPECT_EQ(socket_addr.into_inner(), expected_addr);
}
