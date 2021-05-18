// Very similar to Select.cpp, using a server to read data from a Client.
// This server however, will parse data in the form of an HTTP request, and
// print the generated response on the Server's terminal.

#include <iostream>

#include "../../ASyncServer/src/Server.hpp"
#include "../../HTTP/src/Request.hpp"
#include "../../Result/src/result.hpp"

typedef std::vector<Client*>::iterator client_it;

void handle_client(Client& client) {
    std::string message_received;
    std::string message_sent;

    if (client.state == Client::Read) {
        client.read();
    } else if (client.state == Client::Write && client.request_is_complete()) {
        http::Request::Result req_res = client.generate_request();

        if (req_res.is_err()) {
            std::cout << "Invalid request:" << std::endl;
            std::cout << req_res.unwrap_err() << std::endl;
        } else {
            std::cout << "Valid request: " << std::endl;
            std::cout << req_res.unwrap();
        }
    }
}

int main(void) {
    Server server = Server::bind("localhost:4246").unwrap();
    std::vector<Client*> clients;

    clients.reserve(CLIENT_TOTAL - 1); // 1, as we are using a single TcpListener

    while (true) {
        server.select(clients);

        for (client_it client = clients.begin(); client != clients.end(); client++) {
            handle_client(**client);
        }
    }

    return 0;
}
