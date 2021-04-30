#include <gtest/gtest.h>

#include "../src/Request.hpp"

#include <iostream>

using namespace http;

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

TEST(RequestParser, parse_method) {
    Request::Parser parser;

    parser.parse_line("GET / HTTP/1.1\r\n");
    parser.debug();
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("\r\n");
    EXPECT_TRUE(parser.is_complete());
}
