#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <map>
#include <string>

#include "Request.hpp"
#include "State.hpp"

namespace http {

// Required Response Headers
// - Allow (Valid methods for specified resource - use with err 405) (Allow: GET, HEAD)
// - Content-Language
// - Content-Length
// - Content-Location
// - Content-Type
// - Date (In HTTP-date format)
// - Last-Modified
// - Location
// - Retry-After
// - Server
// - Transfer-Encoding
// - WWW-Authenticate

class Response {
  public:
    friend std::ostream& operator<<(std::ostream&, Response const&);

  public:
    class Builder {
      public:
        Builder(void);
        ~Builder(void);
        Builder(Builder const& src);
        Builder& operator=(Builder const& rhs);

        Builder& header(const char* key, const char* val);
        Builder& header(Slice key, Slice val);
        Builder& header(std::string const& key, std::string const& val);
        Builder& version(Version new_version);
        Builder& body(std::string new_body);
        Builder& append_to_body(Slice const& slice);

        Response build(void);

      private:
        State state_;
        Version version_;
        std::map<std::string, std::string> headers;
        std::string body_;
    };

  public:
    ~Response(void);
    Response(Response const& src);
    Response& operator=(Response const& rhs);

    std::string to_string(void) const;

  private:
    State state;
    Version version;
    std::map<std::string, std::string> headers;
    std::string body;

    Response(void);
    Response(State new_state, Version new_version, std::map<std::string, std::string>& new_headers,
             std::string& new_body);
};

std::ostream& operator<<(std::ostream& o, Response const& response);

} // namespace http

#endif
