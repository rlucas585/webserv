#include <gtest/gtest.h>

#include "../src/Request.hpp"

#include <iostream>

using namespace http;

// Request::Builder is initialized with a default of "GET / HTTP/1.1"

TEST(Request, default_initialization) {
    Request req = Request::Builder().build();

    EXPECT_EQ(req.to_string(), "GET / HTTP/1.1\r\n\r\n");
}

TEST(Request, method) {
    Request req = Request::Builder().method(http::GET).build();

    EXPECT_EQ(req.to_string(), "GET / HTTP/1.1\r\n\r\n");
}

TEST(Request, version) {
    Request req =
        Request::Builder().method(http::GET).method(http::POST).version(http::HTTP_09).build();

    EXPECT_EQ(req.to_string(), "POST / HTTP/0.9\r\n\r\n");
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

TEST(Request, with_body) {
    Request req = Request::Builder()
                      .method(http::GET)
                      .header("Host", "example.com")
                      .header("Content-Length", "27")
                      .body("data=hello\n")
                      .append_to_body("moredata=goodbye")
                      .build();

    EXPECT_EQ(req.to_string(), "GET / HTTP/1.1\r\n"
                               "Content-Length: 27\r\n"
                               "Host: example.com\r\n"
                               "\r\n"
                               "data=hello\n"
                               "moredata=goodbye");
}

TEST(RequestParser, state) {
    State state = OK_200;
    EXPECT_EQ(state, OK_200);
    EXPECT_STREQ(state, "200 OK");

    state = BadRequest_400;
    EXPECT_EQ(state, BadRequest_400);
    EXPECT_STREQ(state, "400 Bad Request");

    std::string str("State is: ");

    str.append(state);
    EXPECT_EQ(str, "State is: 400 Bad Request");
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

    EXPECT_EQ(req.to_string(), "GET / HTTP/1.1\r\n\r\n");
}

TEST(RequestParser, invalid_http) {
    Request::Parser parser;

    parser.parse_line("GET / HTTP/2.0\r\n");
    EXPECT_TRUE(parser.is_complete());

    parser.parse_line("\r\n");
    EXPECT_TRUE(parser.is_complete());

    Request::Result req_res = parser.generate_request();

    EXPECT_TRUE(req_res.is_err());
    EXPECT_EQ(req_res.unwrap_err(), HttpNotSupported_505);
}

TEST(RequestParser, no_crlf) {
    Request::Parser parser;

    parser.parse_line("GET / HTTP/1.0");
    EXPECT_TRUE(parser.is_complete());

    Request::Result req_res = parser.generate_request();
    EXPECT_TRUE(req_res.is_err());
    EXPECT_EQ(req_res.unwrap_err(), BadRequest_400);
}

TEST(RequestParser, empty) {
    Request::Parser parser;

    Request::Result req_res = parser.generate_request();

    EXPECT_FALSE(req_res.is_ok());
    State request_state = req_res.unwrap_err();

    EXPECT_EQ(request_state, BadRequest_400);
}

TEST(RequestParser, ignore_leading_crlf) {
    Request::Parser parser;

    parser.parse_line("\r\n");
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("GET / HTTP/1.0\r\n");
    parser.parse_line("\r\n");
    EXPECT_TRUE(parser.is_complete());
}

TEST(RequestParser, headers) {
    Request::Parser parser;

    parser.parse_line("GET / HTTP/1.1\r\n");
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("Host:          example.com\r\n");
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("Ipv6: 2001:0DB8:AC10:FE01::\r\n");
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("User-Agent:Mozilla FireFox\r\n");
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("User-Agent: Internet Explorer\r\n");
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("\r\n");

    Request::Result req_res = parser.generate_request();

    ASSERT_TRUE(req_res.is_ok());
    Request request = req_res.unwrap();

    EXPECT_EQ(request.to_string(), "GET / HTTP/1.1\r\n"
                                   "Host: example.com\r\n"
                                   "Ipv6: 2001:0DB8:AC10:FE01::\r\n"
                                   "User-Agent: Mozilla FireFox, Internet Explorer\r\n"
                                   "\r\n");
}

TEST(RequestParser, body) {
    Request::Parser parser;

    parser.parse_line("POST / HTTP/1.1\r\n");
    parser.parse_line("Host: example.com\r\n");
    parser.parse_line("User-Agent:Mozilla Firefox\r\n");
    parser.parse_line("Content-Length: 10\r\n");
    parser.parse_line("\r\n");
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("data=hellothere");  // Data after Content-Length is ignored
    parser.parse_line("moredata=iammore"); // Data after Content-Length is ignored

    EXPECT_TRUE(parser.is_complete());

    Request::Result req_res = parser.generate_request();

    ASSERT_TRUE(req_res.is_ok());
    Request request = req_res.unwrap();

    EXPECT_EQ(request.to_string(), "POST / HTTP/1.1\r\n"
                                   "Content-Length: 10\r\n"
                                   "Host: example.com\r\n"
                                   "User-Agent: Mozilla Firefox\r\n"
                                   "\r\n"
                                   "data=hello");
}

TEST(RequestParser, body_chunked) {
    Request::Parser parser;

    parser.parse_line("POST / HTTP/1.1\r\n");
    parser.parse_line("Host: example.com\r\n");
    parser.parse_line("User-Agent:Mozilla Firefox\r\n");
    parser.parse_line("Transfer-Encoding: chunked\r\n");
    parser.parse_line("\r\n");
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("11\r\n");           // Data after Content-Length is ignored
    parser.parse_line("data=hello\n\r\n"); // Data after Content-Length is ignored
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("17\r\n");
    parser.parse_line("moredata=goodbye\n\r\n");
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("0\r\n");
    parser.parse_line("\r\n");
    EXPECT_TRUE(parser.is_complete());

    Request::Result req_res = parser.generate_request();

    ASSERT_TRUE(req_res.is_ok());

    Request request = req_res.unwrap();

    EXPECT_EQ(request.to_string(), "POST / HTTP/1.1\r\n"
                                   "Host: example.com\r\n"
                                   "Transfer-Encoding: chunked\r\n"
                                   "User-Agent: Mozilla Firefox\r\n"
                                   "\r\n"
                                   "data=hello\n"
                                   "moredata=goodbye\n");
}

TEST(RequestParser, body_chunked_no_crlf) {
    Request::Parser parser;

    parser.parse_line("POST / HTTP/1.1\r\n");
    parser.parse_line("Host: example.com\r\n");
    parser.parse_line("User-Agent:Mozilla Firefox\r\n");
    parser.parse_line("Transfer-Encoding: chunked\r\n");
    parser.parse_line("\r\n");
    EXPECT_FALSE(parser.is_complete());

    parser.parse_line("data=hellothere");  // Data after Content-Length is ignored
    parser.parse_line("moredata=iammore"); // Data after Content-Length is ignored

    parser.parse_line("0\r\n");
    parser.parse_line("\r\n");
    EXPECT_TRUE(parser.is_complete());

    Request::Result req_res = parser.generate_request();

    ASSERT_TRUE(req_res.is_err());

    EXPECT_EQ(req_res.unwrap_err(), BadRequest_400);
}
