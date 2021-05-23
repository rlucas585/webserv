// An example of how to use the Server class, to asynchronously handle
// multiple clients.

// Note: This Example used to read lines one by one using a Client, before
// HTTP parsing was implemented. Now Client reading is handled internally,
// and this example doesn't provide much. In future this test could be
// resurrected - potentially by using a different type of Client, and
// either templating the Server or implementing dynamic dispatch.

#include <iostream>

#include "../../ASyncServer/src/Server.hpp"
#include "../../Result/src/result.hpp"

typedef std::vector<Client*>::iterator client_it;

void handle_client(Client& client) {
    std::string message_received;
    std::string message_sent;

    std::cout << "Message from client " << client.fd() << ": " << std::endl;
    client.read();
    http::Request::Result req_res = client.generate_request();

    if (req_res.is_err()) {
        std::cout << "Invalid request:" << std::endl;
        std::cout << req_res.unwrap_err() << std::endl;
    } else {
        std::cout << "Valid request: " << std::endl;
        std::cout << req_res.unwrap();
    }
    client.write(message_sent);
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
