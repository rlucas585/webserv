#include <gtest/gtest.h>

#include "Parser.hpp"
#include "TcpListener.hpp"
#include "WebServ.hpp"

TEST(Configuration, invalid_config) {
    int argc = 2;
    const char* argv[] = {"./SetupTest", "error.conf"};
    WebServ::Config::Result config_parsing = WebServ::Config::parse_config(argc, argv);

    ASSERT_TRUE(config_parsing.is_err());
    EXPECT_EQ(config_parsing.unwrap_err(), "Duplicate server block in configuration");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
