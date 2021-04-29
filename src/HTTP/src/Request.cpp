#include "Request.hpp"

#include <algorithm>

namespace http {

Request::Builder::Builder(void) : method_(GET), uri_("/"), headers() {
}

Request::Builder::~Builder(void) {
}

Request::Builder::Builder(Builder const& src) {
  *this = src;
}

Request::Builder &Request::Builder::operator=(Builder const& rhs) {
  if (&rhs == this) { return *this; }
  method_ = rhs.method_;
  headers = rhs.headers;

  return *this;
}

Request::Builder& Request::Builder::method(Method new_method) {
  method_ = new_method;
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

Request::Result Request::Builder::build(void) {

}

Request::Request(void) : method(INVALID) {
}

Request::~Request(void) {

}

Request::Request(Request const& src) {
  *this = src;
}

Request &Request::operator=(Request const& rhs) {
  if (&rhs == this) { return *this; }

  return *this;
}

std::map<std::string, std::string>& Request::get_headers(void) {
  return headers;
}

const char *Request::valid_headers[] = {
  "Accept-Charset",
  "Accept-Language",
  "Authorization",
  "Content-Length",
  "Content-Type",
  "Date",
  "Host",
  "Referer",
  "Transfer-Encoding",
  "User-Agent",
};

}
