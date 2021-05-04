#include <gtest/gtest.h>

#include <thread>

#include "../../Net/src/TcpListener.hpp"
#include "../src/Client.hpp"

TEST(Client, initialization) {
    Client client;

    EXPECT_EQ(client.state, Client::Inactive);

    // std::vector<Client> clients(3);
    //
    // EXPECT_EQ(clients[0].state, Client::Inactive);
    // EXPECT_EQ(clients[1].state, Client::Inactive);
    // EXPECT_EQ(clients[2].state, Client::Inactive);
}
