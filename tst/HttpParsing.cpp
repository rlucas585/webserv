#include <gtest/gtest.h>

#include "../src/ASyncServer/src/Server.hpp"
#include "../src/HTTP/src/Request.hpp"

#include <chrono>
#include <iostream>
#include <thread>

// Prototypes
typedef std::vector<Client*>::iterator client_it;
static std::string handle_client(Client& client);
static void client_func(const char* address, const char* message);

// Mock class (I think that's the term?)

class RequestTests : public ::testing::Test {
  public:
    Server server;
    std::vector<Client*> clients;
    static constexpr const char* address_info = "localhost:4250";

    std::thread message_send(const char* request_message) {
        return std::thread([request_message](void) {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            client_func(address_info, request_message);
        });
    }

    void expected_request(const char* expected_request) {
        size_t connections_received = 0;
        std::string actual;

        while (connections_received < 1) {
            server.select(clients);

            connections_received += clients.size();
            for (client_it client = clients.begin(); client != clients.end(); client++) {
                actual = handle_client(**client);
                EXPECT_EQ(actual, expected_request);
            }
        }
    }

    void SetUp(void) override {
        TcpListener listener = TcpListener::bind(address_info).unwrap();
        std::vector<TcpListener> listeners;

        listeners.push_back(listener);
        clients.resize(CLIENT_TOTAL - listeners.size());

        server = Server::init(listeners);
    }
};

// client_func() represents an external client
static void client_func(const char* address, const char* message) {
    TcpStream client =
        TcpStream::connect(address).expect("Unable to contact TcpListener from client thread");
    client.write(message);
}

// handle_client() represents the code that will exist in our WebServer.
// In this test file, it simply returns a string either representing the error
// generated from the Request Parsing, or the well-formed request.
static std::string handle_client(Client& client) {
    std::string message_received;
    std::string message_sent;

    client.parse_http();

    http::Request::Result req_res = client.generate_request();

    if (req_res.is_err()) {
        return static_cast<const char*>(req_res.unwrap_err());
    } else {
        return req_res.unwrap().to_string();
    }
}

// Tests

TEST_F(RequestTests, basic_message_parse) {
    std::thread client_thread = message_send("GET / HTTP/1.1\r\nHost:example.com\r\n\r\n");
    expected_request("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
    client_thread.join();
}

TEST_F(RequestTests, empty_message_parse) {
    std::thread client_thread = message_send("aaaaaaa");
    expected_request("BadRequest_400");
    client_thread.join();
}

TEST_F(RequestTests, post_message_parse) {
    std::thread client_thread =
        message_send("POST / HTTP/1.1\r\nContent-Length: 10\r\n\r\ndata=hello");
    expected_request("POST / HTTP/1.1\r\nContent-Length: 10\r\n\r\ndata=hello");
    client_thread.join();
}

// Data beyond Content-Length is ignored
TEST_F(RequestTests, post_message_too_much_data) {
    std::thread client_thread = message_send(
        "POST / HTTP/1.1\r\nContent-Length: 10\r\n\r\ndata=hello_there\r\nmoredata=goodbye");
    expected_request("POST / HTTP/1.1\r\nContent-Length: 10\r\n\r\ndata=hello");
    client_thread.join();
}

TEST_F(RequestTests, post_message_no_length) {
    std::thread client_thread = message_send("POST / HTTP/1.1\r\n\r\ndata=hello\r\n");
    expected_request("LengthRequired_411");
    client_thread.join();
}

TEST_F(RequestTests, post_chunked_parse) {
    std::thread client_thread = message_send("POST / HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n"
                                             "10\r\n"
                                             "data=hello\r\n"
                                             "0\r\n"
                                             "\r\n");
    expected_request("POST / HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\ndata=hello");
    client_thread.join();
}

TEST_F(RequestTests, post_chunked_incomplete) {
    std::thread client_thread = message_send("POST / HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n"
                                             "10\r\n"
                                             "data=hello\r\n");
    expected_request("BadRequest_400");
    client_thread.join();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
