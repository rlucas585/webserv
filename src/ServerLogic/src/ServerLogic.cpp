#include "ServerLogic.hpp"

#include "VirtualServer.hpp"

namespace ServerLogic {

const bool CONNECTION_CLOSE = true;
const bool CONNECTION_KEEP_OPEN = false;

// static LocationSelectResult select_location(http::Request const& request, Layer const*
// server_config) {
//
// }

bool generate_response(http::Request::Result& request_result, SocketAddrV4 const& client_address,
                       Layer const* global_config, std::string& output) {
    http::Request request = request_result.unwrap();
    ConfigSelectResult config_res =
        ServerLogic::select_server_configuration(request.get_host(), client_address, global_config);

    std::cout << "Request: \n" << request;
    if (config_res.is_err()) {
        return serve_error_page(config_res.unwrap_err(), output);
    }

    // Layer const* server_configuration = config_res.unwrap();
    http::Response::Builder response_builder =
        http::Response::Builder().version(request.get_version()).state(http::OK_200);

    // LocationSelectResult location_result = select_location(request, server_configuration);

    response_builder.body("<html>\n"
                          "<h1>WELCOME TO WEBSERV</h1>\n"
                          "</html>\n");
    http::Response response = response_builder.build();
    output.append(response.to_string());

    return CONNECTION_KEEP_OPEN;
}

bool serve_error_page(http::State error, std::string& output) {
    output.clear();
    output.append("<html>\n"
                  "<head><title>");
    output.append(error); // Implicitly converts to char*
    output.append("</title></head>\n");
    output.append("<body>\n");
    output.append("<center><h1>");
    output.append(error);
    output.append("</h1></center>\n");
    output.append("<hr><center>rlucas + sverschu WebServ</center>\n");
    output.append("</body>\n");
    output.append("</html>\n\n");
    return CONNECTION_CLOSE;
}

static bool is_default_server(Layer const* server) {
    Utils::optional<std::string const*> listen_string_opt = server->get_local_value("listen");

    if (!listen_string_opt.has_value()) {
        return false;
    }
    Slice listen_slice = *listen_string_opt.unwrap();
    Slice::Split iter = listen_slice.split();

    if (iter.count_remaining() < 2) {
        return false;
    }
    iter.next();
    Slice next_val = iter.next();
    if (next_val == "default_server") {
        return true;
    }
    return false;
}

void add_matching_host(std::vector<Layer const*>& valid_servers, VirtualServer& candidate_config,
                       Utils::optional<std::string const*> host_name,
                       Layer::ConstIterator& server) {
    if (host_name.has_value()) {
        Utils::optional<std::string const*> v_server_name = candidate_config.name();

        if (v_server_name.has_value()) {
            Slice::Split iter = Slice(*v_server_name.unwrap()).split();
            while (!iter.is_complete()) {
                Slice tmp = iter.next();
                std::cout << "host: " << **host_name << " vs server: " << tmp << std::endl;
                if (**host_name == tmp || tmp == "_") {
                    valid_servers.push_back(&*server);
                    break;
                }
            }
        }
    } else {
        valid_servers.push_back(&*server);
    }
}

ConfigSelectResult select_server_configuration(Utils::optional<std::string const*> host_name,
                                               SocketAddrV4 const& client_address,
                                               Layer const* global_config) {
    Layer const* http_layer = &*(global_config->filter_children("http"));
    std::vector<Layer const*> valid_servers;

    Layer::ConstIterator server = http_layer->filter_children("server");
    for (; server != http_layer->end_children(); server++) {
        VirtualServer tmp = VirtualServer::create(&*server).unwrap();
        if (client_address == tmp.address()) {
            add_matching_host(valid_servers, tmp, host_name, server);
        }
    }
    if (valid_servers.size() == 0) {
        return ConfigSelectResult::Err(http::BadRequest_400);
    }
    for (std::vector<Layer const*>::const_iterator it = valid_servers.begin();
         it != valid_servers.end(); it++) {
        if (is_default_server(*it)) {
            return ConfigSelectResult::Ok(*it);
        }
    }
    return valid_servers.front();
}

} // namespace ServerLogic
