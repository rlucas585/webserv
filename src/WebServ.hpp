#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "ASyncServer/src/Server.hpp"
#include "Config/src/Parser.hpp"
#include "Net/src/TcpListener.hpp"
#include "Result/src/result.hpp"
#include "ServerLogic/src/ServerLogic.hpp"
#include "VirtualServer/src/VirtualServer.hpp"

namespace WebServ {

typedef Utils::result<int, std::string> Result;
typedef std::vector<Client*>::iterator client_it;
struct Config;

Result run(Config& config);

struct Config {
    typedef Utils::result<Config, std::string> Result;

  private:
    typedef Utils::result<size_t, std::string> ServerResult;

  public:
    Config(Config const& other);
    Config& operator=(Config const& rhs);

    Server server;
    std::vector<VirtualServer> virtual_servers;
    std::vector<Client*> active_clients;
    Utils::unique_ptr<Layer> global_config;
    Layer config_global;

  private:
    Config(void);
    Config(Utils::unique_ptr<Layer> parsed_config, std::vector<TcpListener>& listeners,
           std::vector<VirtualServer>& v_servers);

  public:
    static Result parse_config(int argc, char* argv[]);

  private:
    static Result create_servers(Utils::unique_ptr<Layer> parsed_config);
    static ServerResult add_servers_from_http_block(Layer* http,
                                                    std::vector<VirtualServer>& v_servers,
                                                    std::vector<TcpListener>& listeners);
};

} // namespace WebServ

#endif
