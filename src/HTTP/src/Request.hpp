#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>

#include "../../Result/src/result.hpp"
#include "../../Option/src/optional.hpp"
#include "../../Utils/src/Utils.hpp"
#include "../../Str/src/Str.hpp"

#define REQUEST_SIZE_LIMIT 4096

// Request contains a 'data' buffer, this is the maximum size of all Request
// header fields data. If this limit is reached, the server should respond with
// Error 431:
// https://tools.ietf.org/html/rfc6585#section-5

// Additionally, header size limit can be specified. This is generally 4k-8k in 
// nginx, if this limit is broken, server should respond with Error 413:
// https://tools.ietf.org/html/rfc7231#section-6.5.11

namespace http {

enum Method {
  GET,
  HEAD,
  INVALID,
};

// More encodings exist, until these are implemented, respond with 501
enum Encoding {
  Chunked,
  Identity,
};

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

  public:
    class Builder {
      public:
        Builder(void);
        ~Builder(void);
        Builder(Builder const& src);
        Builder &operator=(Builder const& rhs);

        Builder& method(Method new_method);
        Builder& header(std::string const& key, std::string const& val);
        Builder& uri(std::string const& new_uri);

        Result build(void);

      private:
        Method method_;
        std::string uri_; // Consider creating URI class
        std::map<std::string, std::string> headers;
    };

  public:
    Request(void);
    ~Request(void);
    Request(Request const& src);
    Request &operator=(Request const& rhs);

    std::map<std::string, std::string>& get_headers(void);

  private:
    Method method;
    std::map<std::string, std::string> headers;

    Request(Method new_method, std::map<std::string, std::string>& new_headers);

    static const char *valid_headers[];
};

}

#endif
