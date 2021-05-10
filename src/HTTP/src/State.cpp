#include "State.hpp"

namespace http {

State::State(void) : inner(OK_200) {}

State::State(e_State state) : inner(state) {}

State::~State(void) {}

State::State(State const& src) : inner(src.inner) {}

State& State::operator=(State const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    inner = rhs.inner;
    return *this;
}

const char* State::enum_strings[] = {
    "200 OK",
    "400 Bad Request",
    "411 Length Required",
    "414 URI Too Long",
    "431 Request Header Fields Too Large",
    "501 Not Implemented",
    "505 HTTP Version Not Supported",
};

State::operator e_State() const { return inner; }

State::operator const char*() const { return enum_strings[inner]; }

std::ostream& operator<<(std::ostream& o, State const& state) {
    o << static_cast<const char*>(state);
    return o;
}

} // namespace http
