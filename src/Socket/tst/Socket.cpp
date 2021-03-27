/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 13:18:54 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/27 17:06:25 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../../Utils/src/Utils.hpp"
#include "../src/Socket.hpp"

TEST(Socket_tests, creation_test) { Socket socket = Socket::init("127.2.1.1:5050", SOCK_STREAM); }

TEST(Socket_tests, crash_test1) {
    EXPECT_THROW(
        {
            try {
                Socket socket = Socket::init("127.2.1.1:-1", SOCK_STREAM);
            } catch (Utils::runtime_error const& err) {
                EXPECT_STREQ("Invalid port value", err.what());
                throw;
            }
        },
        Utils::runtime_error);
}

TEST(Socket_tests, crash_test2) {
    EXPECT_THROW(
        {
            try {
                Socket socket = Socket::init("127.2.1.1.1:4242", SOCK_STREAM);
            } catch (Utils::runtime_error const& err) {
                EXPECT_STREQ("Invalid string used for Ipv4Addr", err.what());
                throw;
            }
        },
        Utils::runtime_error);
}

TEST(Socket_tests, move_semantics_test) {
    Socket socket1 = Socket::init("127.2.1.1:4242", SOCK_STREAM);
    int initialfd = socket1.into_inner();
    Socket socket2 = socket1;

    EXPECT_NE(socket1.into_inner(), initialfd);
    EXPECT_EQ(socket1.into_inner(), 0);
    EXPECT_EQ(socket2.into_inner(), initialfd);
}

// This test requires a manual 'nc' command currently.
// TODO: Automate this test
TEST(Socket_tests, bind_test) {
    SocketAddrV4 addr = SocketAddrV4::init("localhost:4242");
    Socket socket = Socket::init(addr, SOCK_STREAM);
    int ret;

    Utils::pair<const sockaddr*, socklen_t> addr_info = addr.into_inner();
    const sockaddr* addrp = addr_info.first;
    socklen_t len = addr_info.second;
    ret = bind(socket.into_inner(), addrp, len);
    if (ret == -1) {
        throw Utils::runtime_error("Error binding socket");
    }
    ret = listen(socket.into_inner(), 128); // 128 is the backlog
    if (ret == -1) {
        throw Utils::runtime_error("Error listening from socket");
    }
    sockaddr_storage storage;
    Utils::memset(&storage, 0, sizeof(storage));
    Socket client = socket.accept(reinterpret_cast<sockaddr*>(&storage), &len);
}
