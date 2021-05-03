#include <gtest/gtest.h>

#include "../src/Request.hpp"

#include <iostream>

using namespace http;

// Request::Builder is initialized with a default of "GET / HTTP/1.1"

TEST(Request, default_initialization) {
    Request req = Request::Builder().build();

    EXPECT_EQ(req.to_string(), "GET / HTTP/1.1\r\n\r\n");
}

TEST(Request, with_headers) {
    Request req = Request::Builder()
                      .method(http::GET)
                      .header("Host", "example.com")
                      .header("Content-Length", "1500")
                      .build();

    EXPECT_EQ(req.to_string(), "GET / HTTP/1.1\r\nContent-Length: 1500\r\n"
                               "Host: example.com\r\n\r\n");
}

TEST(RequestParser, state) {
  Request::State state = Request::OK_200;
  EXPECT_EQ(state, Request::OK_200);
  EXPECT_STREQ(state, "OK_200");

  state = Request::BadRequest_400;
  EXPECT_EQ(state, Request::BadRequest_400);
  EXPECT_STREQ(state, "BadRequest_400");

  std::string str("State is: ");

  str.append(state);
  EXPECT_EQ(str, "State is: BadRequest_400");
}

TEST(RequestParser, method) {
    Request::Parser parser;

    parser.parse_line("GET / HTTP/1.1\r\n");
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("\r\n");
    EXPECT_TRUE(parser.is_complete());

    Request::Result req_res = parser.generate_request();

    EXPECT_TRUE(req_res.is_ok());
    Request req = req_res.unwrap();
}

TEST(RequestParser, invalid_http) {
    Request::Parser parser;

    parser.parse_line("GET / HTTP/2.0\r\n");
    EXPECT_TRUE(parser.is_complete());

    parser.parse_line("\r\n");
    EXPECT_TRUE(parser.is_complete());

    Request::Result req_res = parser.generate_request();

    EXPECT_TRUE(req_res.is_err());
    EXPECT_EQ(req_res.unwrap_err(), Request::HttpNotSupported_505);
}

TEST(RequestParser, empty) {
    Request::Parser parser;

    Request::Result req_res = parser.generate_request();

    EXPECT_FALSE(req_res.is_ok());
    Request::State request_state = req_res.unwrap_err();

    EXPECT_EQ(request_state, Request::BadRequest_400);
}

TEST(RequestParser, headers_early) {
  Request::Parser parser;

  parser.parse_line("Host: example.com");
  EXPECT_TRUE(parser.is_complete());

  Request::Result req_res = parser.generate_request();

  EXPECT_EQ(req_res.unwrap_err(), Request::BadRequest_400);
}
