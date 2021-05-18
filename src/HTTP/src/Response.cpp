#include "Response.hpp"

namespace http {

// http::Response Builder class

Response::Builder::Builder(void) : state_(OK_200), version_(HTTP_11) {}

Response::Builder::~Builder(void) {}

Response::Builder::Builder(Builder const& src) { *this = src; }

Response::Builder& Response::Builder::operator=(Builder const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    Builder& ref = const_cast<Builder&>(rhs); // move semantics
    state_ = rhs.state_;
    version_ = rhs.version_;
    headers.swap(ref.headers);
    body_.swap(ref.body_);

    return *this;
}

Response::Builder& Response::Builder::header(const char* key, const char* val) {
    return header(Slice(key), Slice(val));
}

Response::Builder& Response::Builder::header(Slice key, Slice val) {
    std::map<std::string, std::string>::iterator search = headers.find(key);

    if (search != headers.end()) {
        search->second.append(", ").append(val);
    } else {
        headers.insert(std::make_pair(key, val));
    }
    return *this;
}

Response::Builder& Response::Builder::header(std::string const& key, std::string const& val) {
    std::map<std::string, std::string>::iterator search = headers.find(key);

    if (search != headers.end()) {
        search->second += ", " + val;
    } else {
        headers.insert(std::make_pair(key, val));
    }
    return *this;
}

Response::Builder& Response::Builder::state(State new_state) {
    state_ = new_state;
    return *this;
}

Response::Builder& Response::Builder::version(Version new_version) {
    version_ = new_version;
    return *this;
}

Response::Builder& Response::Builder::body(std::string new_body) {
    body_.swap(new_body);
    return *this;
}

Response::Builder& Response::Builder::append_to_body(Slice const& slice) {
    append_slice_to_string(body_, slice);
    return *this;
}

// Build will implicitly add the Content-Length header
Response Response::Builder::build(void) {
    headers.erase("Content-Length");
    headers.insert(std::make_pair("Content-Length", Utils::to_string(body_.size())));

    return Response(state_, version_, headers, body_);
}

// http::Response class

Response::Response(void) : state(OK_200), version(HTTP_11) {}

Response::Response(State new_state, Version new_version,
                   std::map<std::string, std::string>& new_headers, std::string& new_body)
    : state(new_state), version(new_version) {
    headers.swap(new_headers);
    body.swap(new_body);
}

Response::~Response(void) {}

Response::Response(Response const& src) { *this = src; }

Response& Response::operator=(Response const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    Response& mut_ref = const_cast<Response&>(rhs); // move semantics
    state = rhs.state;
    version = rhs.version;
    headers.swap(mut_ref.headers);
    body.swap(mut_ref.body);
    return *this;
}

std::string Response::to_string(void) const {
    std::string output =
        static_cast<std::string>(version) + " " + static_cast<std::string>(state) + "\r\n";
    for (std::map<std::string, std::string>::const_iterator header = headers.begin();
         header != headers.end(); header++) {
        output += header->first + ": " + header->second + "\r\n";
    }
    output += "\r\n";
    output += body;

    return output;
}

std::ostream& operator<<(std::ostream& o, Response const& response) {
    o << response.version << " ";
    o << static_cast<const char*>(response.state) << "\r\n";
    for (std::map<std::string, std::string>::const_iterator header = response.headers.begin();
         header != response.headers.end(); header++) {
        o << header->first << ": " << header->second << "\r\n";
    }
    o << "\r\n";
    o << response.body;

    return o;
}

} // namespace http
