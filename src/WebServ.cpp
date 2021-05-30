#include "WebServ.hpp"
#include "Debugging/src/iostream.hpp"

#include <algorithm>

namespace WebServ {

// Anonymous namespace functions
namespace {
void handle_client(Client& client);
void read_from_client(Client& client);
void write_to_client(Client& client);
} // namespace

// Main Program Loop

WebServ::Result run(Config& config) {
    Server& server = config.server;
    std::vector<Client*>& clients = config.active_clients;

    while (true) {
        server.select(config.active_clients);

        for (client_it client = clients.begin(); client != clients.end(); client++) {
            handle_client(**client);
        }
    }

    return WebServ::Result::Ok(0);
}

// Anonymous namespace functions
namespace {
void handle_client(Client& client) {
    std::string message_received;
    std::string message_sent;

    if (client.state == Client::Read) {
        return read_from_client(client);
    } else if (client.state == Client::Write && client.request_is_complete()) {
        // XXX We only write a response to a client if a request is complete - in order
        // to run effectively with netcat. Potentially there are some issues with
        // this, e.g. maybe a client will repeatedly trigger select() for no need,
        // and it should instead just be disconnected if it attempts to read from
        // server with an incomplete request
        return write_to_client(client);
    }
}

void read_from_client(Client& client) { client.read(); }

void write_to_client(Client& client) {
    http::Request::Result req_res = client.generate_request();
    std::string response_string;

    // TODO actually write a function for this
    // Layer const* select_server_configuration()

    bool response_status = ServerLogic::generate_response(req_res, response_string);

    Debug::cout << "Response: " << response_string;
    Utils::RwResult res = client.write(response_string);

    if (res.is_ok())
        Debug::cout << "bytes sent: " << res.unwrap() << std::endl;

    if (response_status == ServerLogic::CONNECTION_CLOSE) {
        // Client is removed from Server FD_SET before next select() call.
        client.state = Client::Close;
    }
}
} // namespace

// Config

Config::Config(void) {}

Config::Config(Utils::unique_ptr<Layer> parsed_config, std::vector<TcpListener>& listeners,
               std::vector<VirtualServer>& v_servers)
    : global_config(parsed_config) {
    active_clients.reserve(CLIENT_TOTAL - listeners.size());
    server = Server::init(listeners);
    virtual_servers.swap(v_servers);
}

Config::Config(Config const& other) {
    Config& mut_ref = const_cast<Config&>(other);
    global_config.swap(mut_ref.global_config);
    active_clients.swap(mut_ref.active_clients);
    virtual_servers.swap(mut_ref.virtual_servers);
    server = mut_ref.server;
}

WebServ::Config& Config::operator=(Config const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    Config& mut_ref = const_cast<Config&>(rhs);
    global_config.swap(mut_ref.global_config);
    active_clients.swap(mut_ref.active_clients);
    virtual_servers.swap(mut_ref.virtual_servers);
    server = mut_ref.server;
    return *this;
}

Config::Result Config::parse_config(int argc, const char* const* argv) {
    if (argc > 2) {
        std::string output = std::string(argv[0]) + ": invalid number of arguments\n" +
                             "Usage: " + std::string(argv[0]) + " <config_file>";
        return Config::Result(output);
    }

    config::Parser parser;
    config::Parser::Result parse_result = (argc == 2)
                                              ? parser.generate_config_from_file(argv[1])
                                              : parser.generate_config_from_file("default.conf");

    if (parse_result.is_err())
        return Config::Result::Err(parse_result.unwrap_err());

    return Config::create_servers(new Layer(parse_result.unwrap()));
}

Config::Result Config::create_servers(Utils::unique_ptr<Layer> parsed_config) {
    std::vector<VirtualServer> virtual_servers;
    std::vector<TcpListener> listeners;

    Layer::Iterator http = parsed_config->filter_children("http");
    for (; http != parsed_config->end_children(); http++) {
        // add_servers_from_http_block uses virtual_servers and listeners as
        // output arguments
        Utils::result<size_t, std::string> add_servers_result =
            Config::add_servers_from_http_block(&*http, virtual_servers, listeners);

        if (add_servers_result.is_err())
            return Config::Result::Err(add_servers_result.unwrap_err());

        return Config::Result::Ok(Config(parsed_config, listeners, virtual_servers));
    }
    return Config::Result::Err("No http block found in config file");
}

Config::ServerResult Config::add_servers_from_http_block(Layer* http,
                                                         std::vector<VirtualServer>& v_servers,
                                                         std::vector<TcpListener>& listeners) {
    std::vector<SocketAddrV4> existing_addresses;

    Layer::Iterator server = http->filter_children("server");
    for (; server != http->end_children(); server++) {

        // Create a VirtualServer - that contains a pointer to the configuration
        // it uses. Also performs error checking for the "server" block
        VirtualServer::Result v_server_result = VirtualServer::create(&*server);
        if (v_server_result.is_err())
            return ServerResult::Err(v_server_result.unwrap_err());

        VirtualServer new_v_server = v_server_result.unwrap();
        std::vector<VirtualServer>::iterator v_server_search =
            std::find(v_servers.begin(), v_servers.end(), new_v_server);

        if (v_server_search != v_servers.end()) {
            return ServerResult::Err(
                "Duplicate server block in configuration"); // Could make more explicit error
                                                            // message
        }

        v_servers.push_back(new_v_server);

        // Check if the address of the new virtual server already has an associated
        // TcpListener
        std::vector<SocketAddrV4>::iterator search = std::find(
            existing_addresses.begin(), existing_addresses.end(), v_servers.back().address());

        // Add a new TcpListener, and add the new address to the existing_addresses vector
        if (search == existing_addresses.end()) {
            TcpListener::Result new_listener_res = TcpListener::bind(v_servers.back().address());

            if (new_listener_res.is_err())
                return ServerResult::Err(std::string("TcpListener Error: ") +
                                         new_listener_res.unwrap_err());
            listeners.push_back(new_listener_res.unwrap());
            existing_addresses.push_back(listeners.back().get_sock_name());
        }
    }
    return ServerResult::Ok(0); // Success value unused
}

} // namespace WebServ
