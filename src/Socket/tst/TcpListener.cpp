/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpListener.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 10:22:41 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/05 12:11:11 by rlucas        ########   odam.nl         */
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

// The following test is the most complex, and demonstrates how TcpListener will
// be utilized in the project.

// This function simulates clients of the server
static void client_func(int n, const char* address, std::string message) {
    std::string message_sent;
    std::string message_received;
    TcpStream client =
        TcpStream::connect(address).expect("Unable to contact TcpListener from client thread");

    message_sent += std::to_string(n);
    message_sent.append("|");
    message_sent.append(message);
    client.write(message_sent);

    client.read(message_received);
    if (n == 1) {
        EXPECT_EQ(message_received, "Hi first terminal!");
    }
    if (n == 2) {
        EXPECT_EQ(message_received, "Hi second terminal!");
    }
    if (n == 3) {
        EXPECT_EQ(message_received, "Hi third terminal!");
    }
}

static const size_t READ_LIMIT = 512;

typedef std::vector<TcpStream>::iterator client_it;

// This function is receiving client messages, and handling them appropriately
void handle_client(TcpStream& client) {
    std::string message_received;
    std::string message_sent;

    client.read(message_received);

    Str::Split iter(message_received.c_str(), "|");
    std::vector<Str> vec = iter.collect<std::vector<Str> >();

    EXPECT_EQ(vec.size(), 2);
    if (vec[0] == "1") {
        EXPECT_EQ(vec[1], "A message from the first client!");
        message_sent = "Hi first terminal!";
    }
    if (vec[0] == "2") {
        EXPECT_EQ(vec[1], "A message from the second client!");
        message_sent = "Hi second terminal!";
    }
    if (vec[0] == "3") {
        EXPECT_EQ(vec[1], "A message from the third client!");
        message_sent = "Hi third terminal!";
    }

    client.write(message_sent);
}

// Testing the TcpListener with multiple clients
TEST(TcpListener_tests, select_test) {
    const char* address_info = "localhost:4247";
    TcpListener listener = TcpListener::bind(address_info).unwrap();
    int connections_received = 0;

    std::thread clients_thread = std::thread([&address_info](void) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        std::thread(client_func, 3, address_info, "A message from the third client!").detach();
        std::thread(client_func, 1, address_info, "A message from the first client!").detach();
        std::thread(client_func, 2, address_info, "A message from the second client!").detach();
    });
    while (true) {
        std::vector<TcpStream> clients = listener.select().unwrap();

        connections_received += clients.size();
        for (client_it client = clients.begin(); client != clients.end(); client++) {
            handle_client(*client);
        }
        if (connections_received >= 3) {
            break;
        }
    }
    clients_thread.join();
}
