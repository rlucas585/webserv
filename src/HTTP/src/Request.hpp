#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include <string>

#include "../../Option/src/optional.hpp"
#include "../../Result/src/result.hpp"
#include "../../Str/src/Str.hpp"
#include "../../Utils/src/Utils.hpp"

#define URI_SIZE_LIMIT 2048
#define HEADER_SIZE_LIMIT 4096

namespace http {

enum e_Method {
    GET,
    HEAD,
    POST,
    PUT,
};

class Method {
  public:
    Method(void);
    Method(e_Method method);
    ~Method(void);
    Method(Method const& src);
    Method& operator=(Method const& rhs);

    operator e_Method() const;
    operator const char*() const;

  private:
    e_Method inner;

    static const char* enum_strings[];
};

std::ostream& operator<<(std::ostream& o, Method const& method);

enum e_Version {
    HTTP_09,
    HTTP_10,
    HTTP_11,
};

class Version {
  public:
    Version(void);
    Version(e_Version version);
    ~Version(void);
    Version(Version const& src);
    Version& operator=(Version const& rhs);

    operator e_Version() const;
    operator const char*() const;

  private:
    e_Version inner;

    static const char* enum_strings[];
};

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

    typedef Utils::result<Request, State> Result;

    friend std::ostream& operator<<(std::ostream&, Request const&);

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
        Builder& body(std::string new_body);
        Builder& append_to_body(Str const& slice);

        // TODO remove as many of these as possible, when server is ready
        Method const& get_method(void) const;
        std::string const& get_uri(void) const;
        Version const& get_version(void) const;
        Utils::optional<std::string const*> get_header(const char* key) const;
        Utils::optional<std::string const*> get_header(std::string const& key) const;
        std::string const& get_body(void) const;

        Request build(void);

      private:
        Method method_;
        std::string uri_; // Consider creating URI class
        Version version_;
        std::map<std::string, std::string> headers;
        std::string body_;
    };

  public:
    class Parser {
        enum Step {
            Method,
            Headers,
            Processing,
            Body,
            Chunked,
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

        Utils::optional<size_t> ready_for_body(void) const;

        Result generate_request(void);

      private:
        Step step;
        State error;
        Builder builder;
        Utils::optional<size_t> content_length;

        void parse_method(Str line);
        void parse_header(Str line);
        void parse_body(Str line);
        void parse_chunked(Str line);

        void process(void);

        void set_parser_state(Step new_step, State new_state);

        bool line_should_have_CRLF(void) const;
        bool line_has_CRLF(Str const& line) const;
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
    std::string body;

    Request(void);
    Request(Method new_method, std::string& new_uri, Version new_version,
            std::map<std::string, std::string>& new_headers, std::string& new_body);

    static std::map<const Str, Method> create_method_map(void);
    static std::map<const Str, bool> create_header_map(void);
    static std::map<const Str, Version> create_version_map(void);

  public:
    static const std::map<const Str, Method> valid_methods;
    static const std::map<const Str, bool> valid_headers;
    static const std::map<const Str, Version> valid_versions;
};

std::string& operator+(std::string& lhs, Request::State const& rhs);
std::ostream& operator<<(std::ostream& o, Request::State const& state);

} // namespace http

#endif
