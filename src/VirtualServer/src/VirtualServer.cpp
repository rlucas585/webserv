#include "VirtualServer.hpp"

VirtualServer::VirtualServer(void) : config(0) {}

VirtualServer::~VirtualServer(void) {}

VirtualServer::VirtualServer(VirtualServer const& other)
    : listener(other.listener), config(other.config) {}

VirtualServer& VirtualServer::operator=(VirtualServer const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    listener = rhs.listener;
    config = rhs.config;
    return *this;
}

VirtualServer::Result VirtualServer::bind(Layer* server_configuration) {
    if (server_configuration->get_name() != "server") {
        return VirtualServer::Result::Err("Invalid configuration block \'" +
                                          server_configuration->get_name() +
                                          "\' used for Virtual Server");
    }

    Utils::optional<std::string*> listen_res = server_configuration->get_local_value("listen");
    if (!listen_res.has_value())
        return VirtualServer::Result::Err("No listen in server block");

    Slice listen = *(listen_res.unwrap());
    Slice::Split iter = listen.split();
}
