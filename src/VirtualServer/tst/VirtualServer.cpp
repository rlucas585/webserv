#include <gtest/gtest.h>

#include "../../Config/src/Parser.hpp"
#include "../src/VirtualServer.hpp"

class VirtualServerTests : public ::testing::Test {
  public:
    Layer config;
    Layer* httpblock;

    void SetUp(void) override {
        config::Parser parser;

        config::Parser::Result result = parser.generate_config_from_file("config.txt");
        ASSERT_TRUE(result.is_ok());

        config = result.unwrap();

        for (Layer::Iterator block = config.filter_children("http"); block != config.end_children();
             block++) {
            httpblock = &*block;
            break;
        }
    }
};

TEST_F(VirtualServerTests, test) {
    std::vector<VirtualServer> virtual_servers;

    for (Layer::Iterator block = httpblock->filter_children("server");
         block != httpblock->end_children(); block++) {
        VirtualServer::Result result = VirtualServer::create(&*block);
        ASSERT_TRUE(result.is_ok());
        virtual_servers.push_back(result.unwrap());
    }

    EXPECT_EQ(virtual_servers[0].address().to_string(), "127.0.0.1:80");
    EXPECT_EQ(virtual_servers[1].address().to_string(), "127.0.0.1:4505");
    EXPECT_EQ(virtual_servers[2].address().to_string(), "172.17.154.13:80");
}
