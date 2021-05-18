#ifndef SERVERLOGIC_HPP
#define SERVERLOGIC_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "State.hpp"

namespace ServerLogic {

extern const bool CONNECTION_CLOSE;
extern const bool CONNECTION_KEEP_OPEN;

bool generate_response(http::Request::Result& request_result, std::string& output);
bool serve_error_page(http::State error, std::string& output);

} // namespace ServerLogic

#endif
