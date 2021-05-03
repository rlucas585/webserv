#include "Request.hpp"

#include <algorithm>

namespace http {

// 'State' class. Essentially acts as an enum, but with additional functionality,
// primarily the ability to be converted to a const char* with the enum name,
// which allows the use of 'State' as an Error return value in Utils::result.

Request::State::State(void) : inner(OK_200) {}

Request::State::State(State_enum state) : inner(state) {}

Request::State::~State(void) {}

Request::State::State(State const& src) : inner(src.inner) {}

Request::State &Request::State::operator=(State const& rhs) {
  if (&rhs == this) { return *this; }
  inner = rhs.inner;
  return *this;
}

const char* Request::State::enum_strings[] = {
        "OK_200",
        "BadRequest_400",
        "HeaderTooLarge_431",
        "NotImplemented_501",
        "HttpNotSupported_505",
};

Request::State::operator State_enum() const { return inner; }

Request::State::operator const char*() const { return enum_strings[inner]; }

// Below code dealing with enums is a little messy, and strongly coupled.
// Strongly consider changing in future

std::string stringify(Method const& method) {
    switch (method) {
    case GET:
        return "GET";
    case HEAD:
        return "HEAD";
    case POST:
        return "POST";
    default:
        return "";
    }
}

std::ostream& operator<<(std::ostream& o, Method const& method) {
    switch (method) {
    case GET:
        o << "GET";
        break;
    case HEAD:
        o << "HEAD";
        break;
    case POST:
        o << "POST";
        break;
    default:;
    }
    return o;
}

std::string stringify(Version const& version) {
    switch (version) {
    case HTTP_09:
        return "HTTP/0.9";
    case HTTP_10:
        return "HTTP/1.0";
    case HTTP_11:
        return "HTTP/1.1";
    default:
        return "";
        ;
    }
}

std::ostream& operator<<(std::ostream& o, Version const& ver) {
    switch (ver) {
    case HTTP_09:
        o << "HTTP/0.9";
        break;
    case HTTP_10:
        o << "HTTP/1.0";
        break;
    case HTTP_11:
        o << "HTTP/1.1";
        break;
    default:;
    }
    return o;
}

Request::Builder::Builder(void)
    : method_(GET), uri_("/"), version_(HTTP_11), headers() {}

Request::Builder::~Builder(void) {}

Request::Builder::Builder(Builder const& src) { *this = src; }

Request::Builder& Request::Builder::operator=(Builder const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    method_ = rhs.method_;
    uri_ = rhs.uri_;
    version_ = rhs.version_;
    headers = rhs.headers;

    return *this;
}

Request::Builder& Request::Builder::method(Method new_method) {
    method_ = new_method;
    return *this;
}

Request::Builder& Request::Builder::header(const char* key, const char* val) {
    return header(Str(key), Str(val));
}

Request::Builder& Request::Builder::header(Str key, Str val) {
    // if (key.length() + val.length() + 1 > HEADER_SIZE_LIMIT) {
    //     set_state(HeaderTooLarge_431);
    //     return *this;
    // }

    std::map<std::string, std::string>::iterator search = headers.find(key);

    if (search != headers.end()) {
        // If header exists, append new value to end, separated by comma
        // TODO if specific header should not be extensible, set error state
        search->second += ", " + val;
    } else {
        headers.insert(std::make_pair(key, val));
    }
    return *this;
}

Request::Builder& Request::Builder::header(std::string const& key, std::string const& val) {
    // if (key.length() + val.length() + 1 > HEADER_SIZE_LIMIT) {
    //     set_state(HeaderTooLarge_431);
    //     return *this;
    // }
    std::map<std::string, std::string>::iterator search = headers.find(key);

    if (search != headers.end()) {
        // If header exists, append new value to end, separated by comma
        // TODO if specific header should not be extensible, set error state
        search->second += ", " + val;
    } else {
        headers.insert(std::make_pair(key, val));
    }
    return *this;
}

Request::Builder& Request::Builder::uri(std::string const& new_uri) {
    uri_ = new_uri;
    return *this;
}

Request::Builder& Request::Builder::version(Version new_version) {
    version_ = new_version;
    return *this;
}

Method const& Request::Builder::get_method(void) const {
  return method_;
}

std::string const& Request::Builder::get_uri(void) const {
  return uri_;
}

Version const& Request::Builder::get_version(void) const {
  return version_;
}

Utils::optional<std::string const*> Request::Builder::get_header(std::string const& key) const {
    std::map<std::string, std::string>::const_iterator search = headers.find(key);
    if (search == headers.end()) {
      return Utils::nullopt;
    }
    return &search->second;
}

Request Request::Builder::build(void) { return Request(method_, uri_, version_, headers); }

// Request::Parser
//
// This class is the interface between an HTTP Request that arrives in
// text format, and a Request::Builder, which does not contain extensive
// error checking

Request::Parser::Parser(void) : step(Method), error(OK_200) {}

Request::Parser::~Parser(void) {}

Request::Parser::Parser(Parser const& src) { *this = src; }

Request::Parser& Request::Parser::operator=(Parser const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    step = rhs.step;
    error = rhs.error;
    return *this;
}

void Request::Parser::parse_line(Str line) {
    // HTTP Request is invalid without \r\n at the end of each line, unless reading the Body
    if ((step != Error && step != Body && step != Complete) &&
        (line.length() < 2 || Str::newSliceWithOffset(line, line.length() - 2) != "\r\n")) {
        step = Error;
        error = BadRequest_400;
    }
    if (step == Headers && line.length() == 2) {
        step = Processing;
    }

    switch (step) {
    case Method:
        return parse_method(line);
    case Headers:
        return parse_header(line);
    case Processing:
        return process();
    case Body:
        return parse_body(line);
    case Complete:
        return;
    case Error:
        return;
    }
}

bool Request::Parser::is_complete(void) const { return step == Complete || step == Error; }

bool Request::Parser::is_error(void) const { return step == Error; }

Request::Result Request::Parser::generate_request(void) {
  if (error != OK_200) {
    return Request::Result::Err(error);
  } else if (step != Complete) {
    // Message is incomplete, and therefore, badly formed
    return Request::Result::Err(BadRequest_400);
  } else {
    return Request::Result::Ok(builder.build());
  }
}

void Request::Parser::parse_method(Str line) {
    Str::Split iter = line.split();
    Str method = iter.next();
    Str uri = iter.next();
    Str version = iter.next();

    if (!method.isInitialized() || !uri.isInitialized() || !version.isInitialized() ||
        !iter.is_complete()) {
        step = Error;
        error = BadRequest_400;
        return;
    }

    std::map<const Str, http::Method>::const_iterator search = Request::valid_methods.find(method);
    if (search == Request::valid_methods.end()) {
        step = Error;
        error = NotImplemented_501;
        return;
    }
    builder.method(search->second);

    builder.uri(uri);

    std::map<const Str, http::Version>::const_iterator search2 =
        Request::valid_versions.find(version);
    if (search2 == Request::valid_versions.end()) {
        step = Error;
        error = HttpNotSupported_505;
        return;
    }
    builder.version(search2->second);

    // Parser is now moved onto the next step
    step = Headers;
}

void Request::Parser::parse_header(Str line) {
    // TODO implement this
    (void)line;
}

void Request::Parser::parse_body(Str line) {
    // TODO implement this
    (void)line;
}

void Request::Parser::process(void) {
  switch (builder.get_method()) {
    case GET:
      return process_get();
    case HEAD:
      return process_head();
    case POST:
      return process_post();
    case PUT:
      return process_put();
  }
}

void Request::Parser::process_get(void) {
  // Check that all is valid for a GET Request
  // GET shouldn't have a Content-Length, for example, or a Transfer Encoding

  step = Complete;
}

void Request::Parser::process_head(void) {

}

void Request::Parser::process_post(void) {

}

void Request::Parser::process_put(void) {

}

// Request Class

Request::Request(void) : method(GET) {}

Request::~Request(void) {}

Request::Request(Method new_method, std::string& new_uri, Version new_version,
                 std::map<std::string, std::string>& new_headers)
    : method(new_method), uri(), version(new_version), headers() {
    uri.swap(new_uri);
    headers.swap(new_headers);
}

Request::Request(Request const& src) { *this = src; }

Request& Request::operator=(Request const& rhs) {
    if (&rhs == this) {
        return *this;
    }

    return *this;
}

std::string Request::to_string(void) const {
    std::string output = stringify(method) + " " + uri + " " + stringify(version) + "\r\n";

    for (std::map<std::string, std::string>::const_iterator header = headers.begin();
         header != headers.end(); header++) {
        output += header->first + ": " + header->second + "\r\n";
    }
    output += "\r\n";

    return output;
}

std::map<std::string, std::string>& Request::get_headers(void) { return headers; }

// Map definitions - used for simple assignation of enums based on parsed values

std::map<const Str, Method> Request::create_method_map(void) {
    std::map<const Str, Method> m;
    m["GET"] = GET;
    m["HEAD"] = HEAD;
    m["POST"] = POST;
    m["PUT"] = PUT;
    return m;
}
const std::map<const Str, Method> Request::valid_methods = Request::create_method_map();

std::map<const Str, bool> Request::create_header_map(void) {
    std::map<const Str, bool> m;
    m["Accept-Charset"] = true;
    m["Accept-Language"] = true;
    m["Authorization"] = true;
    m["Content-Length"] = true;
    m["Content-Type"] = true;
    m["Date"] = true;
    m["Host"] = true;
    m["Referer"] = true;
    m["Transfer-Encoding"] = true;
    m["User-Agent"] = true;
    return m;
}
const std::map<const Str, bool> Request::valid_headers = Request::create_header_map();

std::map<const Str, Version> Request::create_version_map(void) {
    std::map<const Str, Version> m;
    m["HTTP/0.9"] = HTTP_09;
    m["HTTP/1.0"] = HTTP_10;
    m["HTTP/1.1"] = HTTP_11;
    return m;
}
const std::map<const Str, Version> Request::valid_versions = Request::create_version_map();

std::ostream& operator<<(std::ostream& o, Request const& req) {
    o << req.method << " ";
    o << req.uri << " ";
    o << req.version << "\r\n";
    for (std::map<std::string, std::string>::const_iterator header = req.headers.begin();
         header != req.headers.end(); header++) {
        o << header->first << ": " << header->second << "\r\n";
    }
    o << "\r\n";
    // Add body later

    return o;
}

std::ostream& operator<<(std::ostream& o, Request::State const& state) {
  switch (state) {
    case Request::OK_200:
      o << "OK_200";
      break ;
    case Request::BadRequest_400:
      o << "BadRequest_400";
      break ;
    case Request::HeaderTooLarge_431:
      o << "HeaderTooLarge_431";
      break ;
    case Request::NotImplemented_501:
      o << "NotImplemented_501";
      break ;
    case Request::HttpNotSupported_505:
      o << "HttpNotSupported_505";
      break ;
  }
  return o;
}

} // namespace http
