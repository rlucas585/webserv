// An example of how to use the Server class, to asynchronously handle
// multiple clients.

#include <iostream>

#include "../../ASyncServer/src/Server.hpp"
#include "../../Result/src/result.hpp"

static const size_t READ_LIMIT = 512;

typedef std::vector<Client*>::iterator client_it;

void handle_client(Client& client) {
    std::string message_received;
    std::string message_sent;
    Utils::RwResult res = client.read_line(message_received);
    ssize_t bytes_read = res.unwrap();

    std::cout << "Message from client " << client.fd() << ": " << std::endl;
    while (bytes_read > 0 && message_received != "\r\n") {
        std::cout << "bytes_read = " << bytes_read << std::endl;
        std::cout << message_received;

        message_received.clear();

        res = client.read_line(message_received);
        bytes_read = res.unwrap();
    }
    message_sent = "hello from the serverside\n";

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
