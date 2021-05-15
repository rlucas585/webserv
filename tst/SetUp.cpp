#include <gtest/gtest.h>

#include "Parser.hpp"
#include "Server.hpp"
#include "VirtualServer.hpp"

// Testing WebServ setup

// 1 - call command line parsing logic (file from command line / default config file)
// 2 - Setup any other configuration (Set up TcpListeners & VirtualServers)
// 3 - Call a WebServ::run function
// 4 - Handle error if error occurs (UNNECESSARY)
TEST(ServerSetUp, test) {
    const char* argv[] = {"./WebServ.run", "config.txt"};

    config::Parser parser;

    config::Parser::Result result = parser.generate_config_from_file(argv[1]);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
