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

Request::State& Request::State::operator=(State const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    inner = rhs.inner;
    return *this;
}

const char* Request::State::enum_strings[] = {
    "OK_200",
    "BadRequest_400",
    "LengthRequired_411",
    "URITooLong_414",
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

Request::Builder::Builder(void) : method_(GET), uri_("/"), version_(HTTP_11), headers() {}

Request::Builder::~Builder(void) {}

Request::Builder::Builder(Builder const& src) { *this = src; }

Request::Builder& Request::Builder::operator=(Builder const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    Builder& ref = const_cast<Builder&>(rhs); // move semantics
    method_ = rhs.method_;
    uri_.swap(ref.uri_);
    version_ = rhs.version_;
    headers.swap(ref.headers);
    body_.swap(ref.body_);

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
    std::map<std::string, std::string>::iterator search = headers.find(key);

    if (search != headers.end()) {
        // If header exists, append new value to end, separated by comma
        search->second.append(", ").append(val);
    } else {
        headers.insert(std::make_pair(key, val));
    }
    return *this;
}

Request::Builder& Request::Builder::header(std::string const& key, std::string const& val) {
    std::map<std::string, std::string>::iterator search = headers.find(key);

    if (search != headers.end()) {
        // If header exists, append new value to end, separated by comma
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

Request::Builder& Request::Builder::body(std::string new_body) {
    body_.swap(new_body);
    return *this;
}

Request::Builder& Request::Builder::append_to_body(Str const& slice) {
    append_slice_to_string(body_, slice);
    return *this;
}

Method const& Request::Builder::get_method(void) const { return method_; }

std::string const& Request::Builder::get_uri(void) const { return uri_; }

Version const& Request::Builder::get_version(void) const { return version_; }

Utils::optional<std::string const*> Request::Builder::get_header(const char* key) const {
    std::map<std::string, std::string>::const_iterator search = headers.find(key);
    if (search == headers.end()) {
        return Utils::nullopt;
    }
    return &search->second;
}

Utils::optional<std::string const*> Request::Builder::get_header(std::string const& key) const {
    std::map<std::string, std::string>::const_iterator search = headers.find(key);
    if (search == headers.end()) {
        return Utils::nullopt;
    }
    return &search->second;
}

std::string const& Request::Builder::get_body(void) const { return body_; }

Request Request::Builder::build(void) { return Request(method_, uri_, version_, headers, body_); }

// Request::Parser
//
// This class is the interface between an HTTP Request that arrives in
// text format, and a Request::Builder, which does not contain extensive
// error checking

Request::Parser::Parser(void)
    : step(Method), error(OK_200), builder(), content_length(Utils::nullopt) {}

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
    if (line_should_have_CRLF() && !line_has_CRLF(line)) {
        return set_parser_state(Error, BadRequest_400);
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
    case Chunked:
        return parse_chunked(line);
    case Complete:
        return;
    case Error:
        return;
    }
}

bool Request::Parser::is_complete(void) const { return step == Complete || step == Error; }

bool Request::Parser::is_error(void) const { return step == Error; }

Utils::optional<size_t> Request::Parser::ready_for_body(void) const {
    if (step != Body) {
        return Utils::nullopt;
    }
    return content_length;
}

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

    // Verify that only three whitespace-separated component have been sent
    if (!method.isInitialized() || !uri.isInitialized() || !version.isInitialized() ||
        !iter.is_complete()) {
        return set_parser_state(Error, BadRequest_400);
    }

    // Check that 'method' is valid, then add to the Request::Builder
    std::map<const Str, http::Method>::const_iterator search = Request::valid_methods.find(method);
    if (search == Request::valid_methods.end())
        return set_parser_state(Error, NotImplemented_501);
    builder.method(search->second);

    // Check that the URI is not too large, set value in Request::Builder if valid
    if (uri.length() > URI_SIZE_LIMIT)
        return set_parser_state(Error, URITooLong_414);
    builder.uri(uri);

    // Verify that HTTP version is valid, then set HTTP version in Request::Builder.
    std::map<const Str, http::Version>::const_iterator search2 =
        Request::valid_versions.find(version);
    if (search2 == Request::valid_versions.end())
        return set_parser_state(Error, HttpNotSupported_505);
    builder.version(search2->second);

    // Parser is now moved onto the next step
    step = Headers;
}

void Request::Parser::parse_header(Str line) {
    if (line.trim().length() > HEADER_SIZE_LIMIT)
        return set_parser_state(Error, HeaderTooLarge_431);

    Str::SplitN iter = line.splitn(2, ":");
    Str header_name = iter.next();
    Str header_value = iter.next();

    // Verify that there are two values, separated by ':'
    if (!header_name.isInitialized() || !header_value.isInitialized() || !iter.is_complete())
        return set_parser_state(Error, BadRequest_400);

    builder.header(header_name.trim(), header_value.trim());
}

void Request::Parser::parse_body(Str line) {
    if (!content_length.has_value()) // This *should* be impossible.
        return set_parser_state(Error, BadRequest_400);
    size_t length_remaining = content_length.unwrap();

    if (length_remaining <= line.length()) {
        builder.append_to_body(Str::newSliceWithLength(line, length_remaining));
        content_length = Utils::make_optional<size_t>(0);
        step = Complete;
    } else {
        builder.append_to_body(line);
        content_length = Utils::make_optional<size_t>(length_remaining - line.length());
    }
}

static bool all_are_digits(Str const& s) {
    for (Str::iterator c = s.begin(); c != s.end(); c++) {
        if (!::isdigit(*c))
            return false;
    }
    return true;
}

void Request::Parser::parse_chunked(Str line) {
    if (!content_length.has_value()) {
        line.trim(); // Remove whitespace if reading chunk length
        // New value should be a number denoting next chunk length
        if (!all_are_digits(line)) {
            return set_parser_state(Error, BadRequest_400);
        }
        long next_chunk_length = Utils::atol_length(line.raw(), line.length());
        (void)next_chunk_length;
        content_length = Utils::make_optional(static_cast<size_t>(next_chunk_length));
    } else {
        // Next value is to be added to body. Anything beyond chunk limit is ignored
        size_t next_chunk_length = content_length.unwrap();
        builder.append_to_body(Str::newSliceWithLength(line, next_chunk_length));
        content_length = Utils::nullopt;
        if (next_chunk_length == 0) {
            step = Complete;
        }
    }
}

// The 'process()' function is called when an HTTP Parser receives a CRLF
// that either signals the termination of the Request (e.g. GET/HEAD), or
// separates the Headers and the Message Body (e.g. POST).
void Request::Parser::process(void) {
    Utils::optional<std::string const*> content_length_opt = builder.get_header("Content-Length");
    Utils::optional<std::string const*> transfer_encoding_opt =
        builder.get_header("Transfer-Encoding");

    // If Content-Length is present, prepare to read body
    if (content_length_opt.has_value()) {
        // We shouldn't have both content length AND transfer encoding (RFC 7230 Section 3.3.2)
        if (transfer_encoding_opt.has_value())
            return set_parser_state(Error, BadRequest_400);

        std::string const& length_str = *(content_length_opt.unwrap());
        // Check that string is all digits
        if (length_str.find_first_not_of("0123456789") != std::string::npos)
            return set_parser_state(Error, BadRequest_400);

        long length_long = Utils::atol(length_str.c_str());
        if (length_long < 0)
            return set_parser_state(Error, BadRequest_400);
        content_length = Utils::make_optional(static_cast<size_t>(length_long));

        step = Body;
    } else if (transfer_encoding_opt.has_value()) {
        // TODO Use a mask for different transfer encodings:
        // https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Transfer-Encoding
        std::string const& transfer_encoding_str = *(transfer_encoding_opt.unwrap());
        if (transfer_encoding_str.find("chunked") != std::string::npos) {
            step = Chunked;
        } else
            step = Body;
    } else {
        if (builder.get_method() == POST)
            return set_parser_state(Error, LengthRequired_411);
        step = Complete;
    }
}

void Request::Parser::set_parser_state(Step new_step, State new_state) {
    step = new_step;
    error = new_state;
}

bool Request::Parser::line_should_have_CRLF(void) const {
    return step == Method || step == Headers || step == Processing || step == Chunked;
}

bool Request::Parser::line_has_CRLF(Str const& line) const {
    return line.length() >= 2 && Str::newSliceWithOffset(line, line.length() - 2) == "\r\n";
}

// Request Class

Request::Request(void) : method(GET) {}

Request::~Request(void) {}

Request::Request(Method new_method, std::string& new_uri, Version new_version,
                 std::map<std::string, std::string>& new_headers, std::string& new_body)
    : method(new_method), uri(), version(new_version), headers() {
    uri.swap(new_uri);
    headers.swap(new_headers);
    body.swap(new_body);
}

Request::Request(Request const& src) { *this = src; }

Request& Request::operator=(Request const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    Request& ref = const_cast<Request&>(rhs); // move semantics
    method = rhs.method;
    uri.swap(ref.uri);
    version = rhs.version;
    headers.swap(ref.headers);
    body.swap(ref.body);

    return *this;
}

std::string Request::to_string(void) const {
    std::string output = stringify(method) + " " + uri + " " + stringify(version) + "\r\n";

    for (std::map<std::string, std::string>::const_iterator header = headers.begin();
         header != headers.end(); header++) {
        output += header->first + ": " + header->second + "\r\n";
    }
    output += "\r\n";
    output += body;

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
    o << req.body;

    return o;
}

std::ostream& operator<<(std::ostream& o, Request::State const& state) {
    o << static_cast<const char*>(state);
    return o;
}

} // namespace http
