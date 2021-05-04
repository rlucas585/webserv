#include <gtest/gtest.h>

#include "../src/ASyncServer/src/Server.hpp"
#include "../src/HTTP/src/Request.hpp"

#include <iostream>

class RequestTests : public ::testing::Test {
  public:
    Server server;
    std::vector<Client*> clients;
    const char* address_info;

    void SetUp(void) override {
        address_info = "localhost:4250";
        TcpListener listener = TcpListener::bind(address_info).unwrap();
        std::vector<TcpListener> listeners;

        listeners.push_back(listener);
        clients.resize(CLIENT_TOTAL - listeners.size());

        server = Server::init(listeners);
    }
};

TEST_F(RequestTests, parse_message_from_socket) {
    // TODO Complete test once HTTP Parsing has been integrated into Client
    while (true) {
        break;
        server.select(clients);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
