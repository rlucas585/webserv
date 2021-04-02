/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   SocketAddr.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/26 13:50:51 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/02 23:10:50 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../../Utils/src/Utils.hpp"
#include "../src/SocketAddr.hpp"

TEST(SocketAddr_tests, creation_test) {
    const char* ip_str = "127.2.1.1";
    const u_int16_t port = 1500;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str).unwrap();
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);
    struct sockaddr_in expected_addr;

    expected_addr.sin_family = AF_INET;
    expected_addr.sin_port = htons(port);
    expected_addr.sin_addr.s_addr = inet_addr(ip_str);
    memset(expected_addr.sin_zero, 0, 8);

    EXPECT_EQ(socket_addr, expected_addr);
}

TEST(SocketAddr_tests, family_test) {
    const char* ip_str = "127.2.1.1";
    const u_int16_t port = 1500;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str).unwrap();
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);

    EXPECT_EQ(socket_addr.family(), AF_INET);
}

TEST(SocketAddr_tests, string_creation_test) {
    SocketAddrV4 socket_addr = SocketAddrV4::init("127.2.1.1:1500");
    struct sockaddr_in expected_addr;

    expected_addr.sin_family = AF_INET;
    expected_addr.sin_port = htons(1500);
    expected_addr.sin_addr.s_addr = inet_addr("127.2.1.1");
    memset(expected_addr.sin_zero, 0, 8);

    EXPECT_EQ(socket_addr, expected_addr);
}

// TEST(SocketAddr_tests, implicit_creation_test) {
//     SocketAddrV4 socket_addr = "127.0.0.1:4242";
//     struct sockaddr_in expected_addr;
//
//     expected_addr.sin_family = AF_INET;
//     expected_addr.sin_port = htons(4242);
//     expected_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     memset(expected_addr.sin_zero, 0, 8);
//
//     EXPECT_EQ(socket_addr, expected_addr);
// }

TEST(SocketAddr_tests, string_creation_test2) {
    SocketAddrV4 socket_addr = SocketAddrV4::init("localhost:4242");
    struct sockaddr_in expected_addr;

    expected_addr.sin_family = AF_INET;
    expected_addr.sin_port = htons(4242);
    expected_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(expected_addr.sin_zero, 0, 8);

    EXPECT_EQ(socket_addr, expected_addr);
}

TEST(SocketAddr_tests, crash_test1) {
    EXPECT_THROW(
        {
            try {
                SocketAddrV4 socket_addr = SocketAddrV4::init("127.0.0.1.5:1500");
            } catch (Utils::runtime_error const& err) {
                EXPECT_STREQ("Invalid Str used for Ipv4Addr", err.what());
                throw;
            }
        },
        Utils::runtime_error);
}

TEST(SocketAddr_tests, crash_test2) {
    EXPECT_THROW(
        {
            try {
                SocketAddrV4 socket_addr = SocketAddrV4::init("127.0.0.1:-1");
            } catch (Utils::runtime_error const& err) {
                EXPECT_STREQ("Invalid port value", err.what());
                throw;
            }
        },
        Utils::runtime_error);
}

TEST(SocketAddr_tests, ip_test) {
    const char* ip_str = "127.2.1.1";
    const u_int16_t port = 1500;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str).unwrap();
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
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str).unwrap();
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);

    EXPECT_EQ(socket_addr.port(), port);
}

TEST(SocketAddr_tests, set_ip_test) {
    const char* ip_str = "127.2.1.1";
    const char* ip_str2 = "204.15.172.14";
    const u_int16_t port = 1500;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str).unwrap();
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);

    in_addr expected_ip;
    expected_ip.s_addr = inet_addr(ip_str);

    EXPECT_EQ(socket_addr.ip(), expected_ip);

    // Set new Ip Address (with same port)
    socket_addr.set_ip(Ipv4Addr::init_from_string(ip_str2).unwrap());
    expected_ip.s_addr = inet_addr(ip_str2);

    EXPECT_EQ(socket_addr.ip(), expected_ip);
}

TEST(SocketAddr_tests, set_port_test) {
    const char* ip_str = "127.2.1.1";
    const u_int16_t port = 1500;
    const u_int16_t port2 = 4242;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str).unwrap();
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);

    EXPECT_EQ(socket_addr.port(), port);

    // Change port (with same Ip address)
    socket_addr.set_port(port2);

    EXPECT_EQ(socket_addr.port(), port2);
}

TEST(SocketAddr_tests, into_inner_test) {
    const char* ip_str = "127.2.1.1";
    const u_int16_t port = 1500;
    Ipv4Addr addr = Ipv4Addr::init_from_string(ip_str).unwrap();
    SocketAddrV4 socket_addr = SocketAddrV4::init(addr, port);
    struct sockaddr_in expected_addr;

    expected_addr.sin_family = AF_INET;
    expected_addr.sin_port = htons(port);
    expected_addr.sin_addr.s_addr = inet_addr(ip_str);
    memset(expected_addr.sin_zero, 0, 8);

    const sockaddr_in* actual_addr = reinterpret_cast<const sockaddr_in*>(socket_addr.into_inner().first);

    EXPECT_EQ(*actual_addr, expected_addr);
}
