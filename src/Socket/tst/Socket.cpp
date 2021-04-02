/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 13:18:54 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/02 23:11:35 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../../Utils/src/Utils.hpp"
#include "../src/Socket.hpp"
#include <algorithm>
#include <thread>
#include <vector>

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
                EXPECT_STREQ("Invalid Str used for Ipv4Addr", err.what());
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
}

// Code here is messy, TcpListener should internalise much of the logic in future
TEST(Socket_tests, connection_test) {
    SocketAddrV4 addr = SocketAddrV4::init("localhost:4243");
    Socket server = Socket::init(addr, SOCK_STREAM);
    int ret;

    // sockaddr* and socklen_t are required for socket functions
    Utils::pair<const sockaddr*, socklen_t> addr_info = addr.into_inner();
    const sockaddr* addrp = addr_info.first;
    socklen_t len = addr_info.second;

    // Ensure quick rebinding
    ret = 1;
    if (setsockopt(server.into_inner(), SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(sockaddr_in)) == -1) {
        throw Utils::runtime_error(std::string("Error in setsockopt(): ") + strerror(errno));
    }

    ret = bind(server.into_inner(), addrp, len);
    if (ret == -1) {
        throw Utils::runtime_error("Error binding socket");
    }
    ret = listen(server.into_inner(), 128); // 128 is the backlog
    if (ret == -1) {
        throw Utils::runtime_error("Error listening from socket");
    }
    std::thread server_thread = std::thread([&server](void) {
        size_t max_buf = 512;
        socklen_t server_len;
        sockaddr_storage storage;
        Utils::memset(&storage, 0, sizeof(storage));
        Socket client;
        client = server.accept(reinterpret_cast<sockaddr*>(&storage), &server_len);
        std::vector<char> buf(max_buf, 0);

        if (client.read(&buf[0], max_buf) == -1) {
            Utils::runtime_error(std::string("Error in recv(): ") + strerror(errno));
        }
        std::string message_received(buf.begin(), buf.end());
        message_received.erase(std::remove(message_received.begin(), message_received.end(), '\0'),
                               message_received.end());
        EXPECT_EQ(message_received, "hello from the other side");
    });
    // Sleep to ensure server thread has time to reach accept().
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    Socket client = Socket::init(addr, SOCK_STREAM);
    sockaddr_storage storage;
    Utils::memset(&storage, 0, sizeof(storage));
    if (client.connect(addrp, len) == false) {
        std::cout << "Error connecting to server" << std::endl;
    }
    std::string message_sent("hello from the other side");
    if (client.send(message_sent.c_str(), message_sent.size()) == -1) {
        throw Utils::runtime_error(std::string("Error in send(): ") + strerror(errno));
    }
    server_thread.join();
}
