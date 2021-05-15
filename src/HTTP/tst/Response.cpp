#include <gtest/gtest.h>

#include "Response.hpp"

#include <iostream>

using namespace http;

TEST(Response, default_initialization) {
    Response response = Response::Builder().build();

    EXPECT_EQ(response.to_string(), "HTTP/1.1 200 OK\r\n\r\n");
}

TEST(Response, version) {
    Response response = Response::Builder().version(HTTP_09).build();

    EXPECT_EQ(response.to_string(), "HTTP/0.9 200 OK\r\n\r\n");
}

TEST(Response, headers) {
    Response response = Response::Builder()
                            .header("Content-Encoding", "gzip")
                            .header("Accept-Ranges", "bytes")
                            .header("Age", "86415")
                            .header("Cache-Control", "max-age=604800")
                            .header("Etag", "\"3147526947\"")
                            .build();

    EXPECT_EQ(response.to_string(), "HTTP/1.1 200 OK\r\n"
                                    "Accept-Ranges: bytes\r\n"
                                    "Age: 86415\r\n"
                                    "Cache-Control: max-age=604800\r\n"
                                    "Content-Encoding: gzip\r\n"
                                    "Etag: \"3147526947\"\r\n"
                                    "\r\n");
}

TEST(Response, body) {
    Response response = Response::Builder()
                            .header("Age", "86415")
                            .header("Content-Length", "40")
                            .append_to_body("This is a message body from an http resp")
                            .build();

    EXPECT_EQ(response.to_string(), "HTTP/1.1 200 OK\r\n"
                                    "Age: 86415\r\n"
                                    "Content-Length: 40\r\n"
                                    "\r\n"
                                    "This is a message body from an http resp");
}
