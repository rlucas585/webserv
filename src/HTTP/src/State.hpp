#ifndef STATE_HPP
#define STATE_HPP

#include <iostream>
#include <string>

namespace http {
enum e_State {
    OK_200,
    BadRequest_400,
    LengthRequired_411,
    URITooLong_414,
    HeaderTooLarge_431,
    NotImplemented_501,
    HttpNotSupported_505,
};

class State {
  public:
    State(void);
    State(e_State state);
    ~State(void);
    State(State const& src);
    State& operator=(State const& rhs);

    operator e_State() const;
    operator const char*() const;

  private:
    e_State inner;

    static const char* enum_strings[];
};
std::string& operator+(std::string& lhs, State const& rhs);
std::ostream& operator<<(std::ostream& o, State const& state);

} // namespace http

#endif
