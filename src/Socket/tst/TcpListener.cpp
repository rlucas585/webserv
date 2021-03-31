/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpListener.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 10:22:41 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/31 14:24:35 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../../Utils/src/Utils.hpp"
#include "../src/TcpListener.hpp"
#include <algorithm>
#include <thread>
#include <vector>

TEST(TcpListener_tests, creation_test) {
    TcpListener     listener = TcpListener::bind("127.0.0.1:7878");
}

TEST(TcpListener_tests, crash_test1) {
    EXPECT_THROW(
        {
            try {
            TcpListener     listener = TcpListener::bind("127.2.1.1:::::7878");
            } catch (Utils::runtime_error const& err) {
                EXPECT_STREQ("Invalid Str used for SocketAddrV4", err.what());
                throw;
            }
        },
        Utils::runtime_error);
}

TEST(TcpListener_tests, crash_test2) {
    EXPECT_THROW(
        {
            try {
            TcpListener     listener = TcpListener::bind("127.2.1.1:-1");
            } catch (Utils::runtime_error const& err) {
                EXPECT_STREQ("Invalid port value", err.what());
                throw;
            }
        },
        Utils::runtime_error);
}

TEST(TcpListener_tests, crash_test3) {
    EXPECT_THROW(
        {
            try {
            TcpListener     listener = TcpListener::bind("127.2.1.1.1:4242");
            } catch (Utils::runtime_error const& err) {
                EXPECT_STREQ("Invalid string used for Ipv4Addr", err.what());
                throw;
            }
        },
        Utils::runtime_error);
}

TEST(TcpListener_tests, move_semantics_test) {
    TcpListener listener1 = TcpListener::bind("127.2.1.1:4242");
    int initialfd = listener1.socket().into_inner();
    TcpListener listener2 = listener1;

    EXPECT_NE(listener1.socket().into_inner(), initialfd);
    EXPECT_EQ(listener1.socket().into_inner(), 0);
    EXPECT_EQ(listener2.socket().into_inner(), initialfd);
}

TEST(TcpListener_tests, connection_test) {
    SocketAddrV4 addr = SocketAddrV4::init("localhost:4242");
    TcpListener listener = TcpListener::bind(addr);

    std::thread server_thread = std::thread([&listener](void) {
            TcpStream thread_client = listener.accept().first;
            std::string message_received;
            char        buffer[30];

            thread_client.read(reinterpret_cast<void*>(buffer), 30);
            message_received = buffer;
            EXPECT_EQ(message_received, "hello from the other side");
            });
    // Sleep to allow server thread time to setup (probably not necessary)
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    TcpStream   client = TcpStream::connect("localhost:4242");
    if (client.write("hello from the other side") == -1) {
        throw Utils::runtime_error(std::string("Error in write(): ") + strerror(errno));
    }
    server_thread.join();
}
