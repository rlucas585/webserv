#include "VirtualServer.hpp"

VirtualServer::~VirtualServer(void) {}

VirtualServer::VirtualServer(Layer const* server_configuration) : config(server_configuration) {}

VirtualServer::VirtualServer(VirtualServer const& other) : config(other.config) {}

VirtualServer& VirtualServer::operator=(VirtualServer const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    config = rhs.config;
    return *this;
}

VirtualServer::Result VirtualServer::create(Layer const* server_configuration) {
    if (server_configuration->get_name() != "server") {
        return VirtualServer::Result::Err("Invalid configuration block \'" +
                                          server_configuration->get_name() +
                                          "\' used for Virtual Server");
    }
    Utils::optional<std::string const*> listen_res =
        server_configuration->get_local_value("listen");
    if (!listen_res.has_value())
        return VirtualServer::Result::Err("No listen in server block");

    if (validate_listen(*listen_res.unwrap()))
        return VirtualServer::Result::Err("Invalid listen in server block");

    return VirtualServer(server_configuration);
}

SocketAddrV4 VirtualServer::address(void) const {
    // unwrap is safe because we validated presence of listen in VirtualServer::create
    Slice listen_string = *(config->get_local_value("listen").unwrap());
    Slice host_and_port = listen_string.split().next();

    SocketAddrV4::Result socket_address_result = SocketAddrV4::init(host_and_port);

    // If statement only entered if listen was a number alone, such as "listen 80;",
    // instead of "listen 192.168.0.1:80" for example.
    if (socket_address_result.is_err()) {
        u_int16_t port =
            static_cast<u_int16_t>(Utils::atol_length(host_and_port.raw(), host_and_port.length()));
        // If no IP address specified, then bind to 0.0.0.0:
        // https://stackoverflow.com/questions/20778771/
        return SocketAddrV4::init(Ipv4Addr::init_from_string("0.0.0.0").unwrap(), port);
    }
    return socket_address_result.unwrap();
}

bool VirtualServer::validate_listen(Slice listen) {
    Slice::Split iter = listen.split();
    Slice host_and_port = iter.next();

    SocketAddrV4::Result socket_address_result = SocketAddrV4::init(host_and_port);

    if (socket_address_result.is_err()) {
        for (Slice::iterator c = host_and_port.begin(); c != host_and_port.end(); c++) {
            if (!Utils::isnum(*c))
                return INVALID;
        }
        long port = Utils::atol_length(host_and_port.raw(), host_and_port.length());

        if (!(port >= 1 && port <= 65535))
            return INVALID;
        return VALID;
    }
    if (!iter.is_complete() && iter.next().trim() != "default_server")
        return INVALID;
    return VALID;
}

bool VirtualServer::INVALID = true;
bool VirtualServer::VALID = false;
