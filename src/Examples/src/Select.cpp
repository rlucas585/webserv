// An example of how to use the Server class, to asynchronously handle
// multiple clients.

#include <iostream>

#include "../../ASyncServer/src/Server.hpp"
#include "../../Result/src/result.hpp"

typedef std::vector<Client*>::iterator client_it;

void handle_client(Client& client) {
    std::string message_received;
    std::string message_sent;

    std::cout << "Message from client " << client.fd() << ": " << std::endl;
    while (!client.eof()) {
        client.read_line(message_received);
        std::cout << message_received;
        message_received.clear();
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
