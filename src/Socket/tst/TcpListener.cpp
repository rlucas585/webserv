/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpListener.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 10:22:41 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/03 22:03:02 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../../Utils/src/Utils.hpp"
#include "../src/TcpListener.hpp"
#include <algorithm>
#include <thread>
#include <vector>

TEST(TcpListener_tests, creation_test) {
    TcpListener listener = TcpListener::bind("127.0.0.1:7878").unwrap();
}

TEST(TcpListener_tests, crash_test1) {
    TcpListener::Result res = TcpListener::bind("127.2.1.1:::::7878");

    EXPECT_EQ(res, TcpListener::Result::Err("Invalid Str used for SocketAddrV4"));
}

TEST(TcpListener_tests, crash_test2) {
    TcpListener::Result res = TcpListener::bind("127.2.1.1:-1");

    EXPECT_EQ(res, TcpListener::Result::Err("Invalid port value"));
}

TEST(TcpListener_tests, crash_test3) {
    TcpListener::Result res = TcpListener::bind("127.2.1.1.1:4242");

    EXPECT_EQ(res, TcpListener::Result::Err("Invalid Str used for Ipv4Addr"));
}

TEST(TcpListener_tests, move_semantics_test) {
    TcpListener listener1 = TcpListener::bind("127.2.1.1:4244").unwrap();
    int initialfd = listener1.socket().into_inner();
    TcpListener listener2 = listener1;

    EXPECT_NE(listener1.socket().into_inner(), initialfd);
    EXPECT_EQ(listener1.socket().into_inner(), 0);
    EXPECT_EQ(listener2.socket().into_inner(), initialfd);
}

TEST(TcpListener_tests, connection_test) {
    TcpListener listener = TcpListener::bind("localhost:4245").unwrap();

    std::thread server_thread = std::thread([&listener](void) {
        TcpStream thread_client = listener.accept().unwrap().first;
        std::string message_received;
        char buffer[30];
        Utils::memset(buffer, 0, 30);

        thread_client.read(reinterpret_cast<void*>(buffer), 30);
        message_received = buffer;
        EXPECT_EQ(message_received, "hello from the other side");
    });
    // Sleep to allow server thread time to setup (probably not necessary)
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    TcpStream client =
        TcpStream::connect("localhost:4245").expect("Unable to connect to localhost:4245");
    client.write("hello from the other side").expect("Error writing to localhost:4245");
    server_thread.join();
}
