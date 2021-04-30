#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include <string>

#include "../../Option/src/optional.hpp"
#include "../../Result/src/result.hpp"
#include "../../Str/src/Str.hpp"
#include "../../Utils/src/Utils.hpp"

#define HEADER_SIZE_LIMIT 4096

namespace http {

enum Method {
    GET,
    HEAD,
    POST,
    PUT,
};

std::string stringify(Method const& method);
std::ostream& operator<<(std::ostream& o, Method const& method);

// More encodings exist, until these are implemented, respond with 501
enum Encoding {
    Chunked,
    Identity,
};

enum Version {
    HTTP_09,
    HTTP_10,
    HTTP_11,
};

std::string stringify(Version const& version);
std::ostream& operator<<(std::ostream& o, Version const& ver);

// Required Request Headers:
// - Accept-Charset
// - Accept-Language
// - Authorization
// - Content-Length
// - Content-Type
// - Date
// - Host
// - Referer
// - Transfer-Encoding
// - User-Agent

class Request {
  public:
    typedef Utils::result<Request, std::string> Result;

    friend std::ostream& operator<<(std::ostream&, Request const&);

    enum State {
        OK_200,
        BadRequest_400,
        HeaderTooLarge_431,
        NotImplemented_501,
        HttpNotSupported_505,
    };

  public:
    class Builder {
      public:
        Builder(void);
        ~Builder(void);
        Builder(Builder const& src);
        Builder& operator=(Builder const& rhs);

        Builder& method(Method new_method);
        Builder& header(const char* key, const char* val);
        Builder& header(Str key, Str val);
        Builder& header(std::string const& key, std::string const& val);
        Builder& uri(std::string const& new_uri);
        Builder& version(Version new_version);

        Request build(void);

      private:
        Method method_;
        std::string uri_; // Consider creating URI class
        Version version_;
        std::map<std::string, std::string> headers;
        State state;

        bool is_invalid(void) const;
        void set_state(State new_state);
    };

  public:
    class Parser {
        enum Step {
            Method,
            Headers,
            Processing,
            Body,
            Complete,
            Error,
        };

      public:
        Parser(void);
        ~Parser(void);
        Parser(Parser const& src);
        Parser& operator=(Parser const& rhs);

        void parse_line(Str line);
        bool is_complete(void) const;
        bool is_error(void) const;

        // for development only, obviously
        void debug(void) const;

      private:
        Step step;
        State error;
        Builder builder;

        void parse_method(Str line);
        void parse_header(Str line);
        void parse_body(Str line);

        void process(void);
    };

  public:
    ~Request(void);
    Request(Request const& src);
    Request& operator=(Request const& rhs);

    std::string to_string(void) const;
    std::map<std::string, std::string>& get_headers(void);

  private:
    Method method;
    std::string uri; // Consider creating URI class
    Version version;
    std::map<std::string, std::string> headers;

    Request(void);
    Request(Method new_method, std::string& new_uri, Version new_version,
            std::map<std::string, std::string>& new_headers);

    static std::map<const Str, Method> create_method_map(void);
    static std::map<const Str, bool> create_header_map(void);
    static std::map<const Str, Version> create_version_map(void);

  public:
    static const std::map<const Str, Method> valid_methods;
    static const std::map<const Str, bool> valid_headers;
    static const std::map<const Str, Version> valid_versions;
};

} // namespace http

#endif
