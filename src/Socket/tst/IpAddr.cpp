/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IpAddr.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/24 16:18:04 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/26 12:42:56 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/IpAddr.hpp"
#include <arpa/inet.h>
#include <cstdio>
#include <gtest/gtest.h>

template <typename T, size_t size>
::testing::AssertionResult ArraysMatch(const T (&expected)[size], const T (&actual)[size]) {
    for (size_t i = 0; i < size; i++) {
        if (expected[i] != actual[i]) {
            return ::testing::AssertionFailure()
                   << "array[" << i << "] (" << actual[i] << ") != expected[" << i << "] (" << expected[i] << ")";
        }
    }
    return ::testing::AssertionSuccess();
}

// Troubleshooting function -- uncomment if required
// static void    print_bytes(u_int8_t *ptr, size_t count) {
//     for (size_t i = 0; i < count; i++) {
//         printf("%02hhX", *(ptr + i));
//         if (i + 1 < count)
//             printf("|");
//     }
//     printf("\n");
// }

TEST(IpAddr_tests, big_endian_test) {
    u_int8_t bytes[4] = {127, 2, 1, 1};
    in_addr real_addr;

    real_addr.s_addr = inet_addr("127.2.1.1");

    EXPECT_EQ(real_addr.s_addr, IpAddr::u32_convert_to_big_endian(bytes));
}

TEST(IpAddr_tests, htons_test) {
    u_int16_t port = 1500;
    struct sockaddr_in actual_addr;
    struct sockaddr_in expected_addr;

    actual_addr.sin_port = IpAddr::host_to_network_short(port);
    expected_addr.sin_port = htons(port);

    EXPECT_EQ(actual_addr.sin_port, expected_addr.sin_port);
}

TEST(IpAddr_tests, htonl_test) {
    u_int32_t num = 1872938;
    u_int32_t actual;
    u_int32_t expected;

    actual = IpAddr::host_to_network_long(num);
    expected = htonl(num);

    EXPECT_EQ(actual, expected);
}

TEST(IpAddr_tests, ntohl_test) {
    u_int32_t num = 1872938;
    u_int32_t actual;
    u_int32_t expected;

    actual = IpAddr::network_to_host_long(num);
    expected = ntohl(num);

    EXPECT_EQ(actual, expected);
}

TEST(IpAddr_tests, ntohs_test) {
    u_int16_t num = 1500;
    u_int16_t actual;
    u_int16_t expected;

    actual = IpAddr::network_to_host_short(num);
    expected = ntohs(num);

    EXPECT_EQ(actual, expected);
}

TEST(IpAddr_tests, creation_with_string_test) {
    Ipv4Addr addr = Ipv4Addr::init_from_string("127.2.1.1");
    u_int8_t expected_bytes[] = {127, 2, 1, 1};
    u_int8_t actual_bytes[4];

    addr.octets(actual_bytes);
    EXPECT_TRUE(ArraysMatch(expected_bytes, actual_bytes));
}

TEST(IpAddr_tests, creation_with_bytes_test) {
    u_int8_t initialisation_bytes[] = {255, 4, 168, 15};
    Ipv4Addr addr = Ipv4Addr::init_from_bytes(initialisation_bytes);
    u_int8_t expected_bytes[] = {255, 4, 168, 15};
    u_int8_t actual_bytes[4];

    addr.octets(actual_bytes);
    EXPECT_TRUE(ArraysMatch(expected_bytes, actual_bytes));
}

TEST(IpAddr_tests, is_unspecified_test) {
    Ipv4Addr addr1 = Ipv4Addr::init_from_string("0.0.0.0");
    Ipv4Addr addr2 = Ipv4Addr::init_from_string("127.0.0.1");
    Ipv4Addr addr3 = Ipv4Addr::init_from_string("167.151.23.219");

    EXPECT_TRUE(addr1.is_unspecified());
    EXPECT_FALSE(addr2.is_unspecified());
    EXPECT_FALSE(addr3.is_unspecified());
}

TEST(IpAddr_tests, is_loopback_test) {
    Ipv4Addr addr1 = Ipv4Addr::init_from_string("0.0.0.0");
    Ipv4Addr addr2 = Ipv4Addr::init_from_string("127.0.0.1");
    Ipv4Addr addr3 = Ipv4Addr::init_from_string("167.151.23.219");

    EXPECT_FALSE(addr1.is_loopback());
    EXPECT_TRUE(addr2.is_loopback());
    EXPECT_FALSE(addr3.is_loopback());
}

TEST(IpAddr_tests, comparison_test) {
    Ipv4Addr addr1 = Ipv4Addr::init_from_string("0.0.0.0");
    Ipv4Addr addr2 = Ipv4Addr::init_from_string("127.0.0.1");
    Ipv4Addr addr3 = Ipv4Addr::init_from_string("167.151.23.219");
    Ipv4Addr addr4 = Ipv4Addr::init_from_string("167.151.23.219");

    EXPECT_NE(addr1, addr2);
    EXPECT_EQ(addr3, addr4);
}
