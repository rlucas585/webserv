#include <iostream>

#include "Config/src/Parser.hpp"
#include "Net/src/TcpListener.hpp"
#include "WebServ.hpp"

int main(int argc, char* argv[]) {
    WebServ::Config::Result config_parsing = WebServ::Config::parse_config(argc, argv);

    if (config_parsing.is_err()) {
        std::cerr << config_parsing.unwrap_err() << std::endl;
        return 1;
    }

    WebServ::Config config = config_parsing.unwrap();
    WebServ::Result program_result = WebServ::run(config);

    if (program_result.is_err()) {
        std::cerr << program_result.unwrap_err();
        return 1;
    }

    return 0;
}
