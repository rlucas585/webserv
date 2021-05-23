#ifndef SERVERLOGIC_HPP
#define SERVERLOGIC_HPP

#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "State.hpp"
#include "result.hpp"

#include <string>

namespace ServerLogic {

extern const bool CONNECTION_CLOSE;
extern const bool CONNECTION_KEEP_OPEN;

typedef Utils::result<Layer const*, http::State> ConfigSelectResult;
typedef Utils::result<Layer const*, http::State> LocationSelectResult;

bool generate_response(http::Request::Result& request_result, SocketAddrV4 const& client_address,
                       Layer const* global_config, std::string& output);
bool serve_error_page(http::State error, std::string& output);
ConfigSelectResult select_server_configuration(Utils::optional<std::string const*> host_name,
                                               SocketAddrV4 const& client_address,
                                               Layer const* global_config);

} // namespace ServerLogic

#endif
