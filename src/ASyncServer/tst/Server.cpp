#include <gtest/gtest.h>

#include "../src/Server.hpp"

#include <thread>
#include <vector>

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

// This function is receiving client messages, and handling them appropriately
void handle_client(TcpStream& client) {
    std::string message_received;
    std::string message_sent;

    client.read(message_received);

    std::cout << "client fd: " << client.fd() << ", message received = " << message_received
              << ", message size: " << message_received.size() << std::endl;

    Slice::Split iter(message_received.c_str(), "|");
    std::vector<Slice> vec = iter.collect<std::vector<Slice> >();

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

typedef std::vector<Client*>::iterator client_it;

// XXX A note on these tests: They are not an exact simulation of how the
// ASyncServer should run - as they immediately send data back to the client.
// They do however, show the general manner to handle clients: Using an output
// parameter: a std::vector of client pointers. This vector can then be
// iterated over to handle clients.

TEST(Server, select) {
    const char* address_info = "localhost:4247";
    TcpListener listener = TcpListener::bind(address_info).unwrap();
    std::vector<TcpListener> listeners;
    std::vector<Client*> clients;

    listeners.push_back(listener);
    clients.resize(CLIENT_TOTAL - listeners.size());

    Server server = Server::init(listeners);
    int connections_received = 0;

    std::thread clients_thread = std::thread([&address_info](void) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        std::thread handle1 =
            std::thread(client_func, 3, address_info, "A message from the third client!");
        std::thread handle2 =
            std::thread(client_func, 1, address_info, "A message from the first client!");
        std::thread handle3 =
            std::thread(client_func, 2, address_info, "A message from the second client!");
        handle1.join();
        handle2.join();
        handle3.join();
    });

    while (true) {
        server.select(clients);

        connections_received += clients.size();
        // std::cout << "connections_received = " << connections_received << std::endl;
        for (client_it client = clients.begin(); client != clients.end(); client++) {
            handle_client((*client)->stream());
        }
        if (connections_received >= 3) {
            break;
        }
    }
    clients_thread.join();
}

TEST(Server, multiple_addresses) {
    const char* address1 = "localhost:4248";
    const char* address2 = "0.0.0.0:4249";
    TcpListener listener1 = TcpListener::bind(address1).unwrap();
    TcpListener listener2 = TcpListener::bind(address2).unwrap();
    std::vector<TcpListener> listeners;
    std::vector<Client*> clients;

    listeners.push_back(listener1);
    listeners.push_back(listener2);
    clients.resize(CLIENT_TOTAL - listeners.size());

    Server server = Server::init(listeners);
    int connections_received = 0;

    std::thread clients_thread = std::thread([&](void) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        std::thread handle1 =
            std::thread(client_func, 3, address1, "A message from the third client!");
        std::thread handle2 =
            std::thread(client_func, 1, address2, "A message from the first client!");
        std::thread handle3 =
            std::thread(client_func, 2, address1, "A message from the second client!");
        handle1.join();
        handle2.join();
        handle3.join();
    });

    while (true) {
        server.select(clients);

        connections_received += clients.size();
        for (client_it client = clients.begin(); client != clients.end(); client++) {
            handle_client((*client)->stream());
        }
        if (connections_received >= 3) {
            break;
        }
    }
    clients_thread.join();
}
