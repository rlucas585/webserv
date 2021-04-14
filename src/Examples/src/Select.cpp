/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Select.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/05 01:09:53 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/05 01:12:42 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// An example of how to use the 'select' function of the TcpListener class
// to asynchronously handle multiple clients.

#include <iostream>

#include "../../Socket/src/TcpListener.hpp"
#include "../../Socket/src/TcpStream.hpp"

static const size_t READ_LIMIT = 512;

typedef std::vector<TcpStream>::iterator client_it;

void handle_client(TcpStream& client) {
    std::vector<char> buffer(READ_LIMIT, 0);

    client.read(&buffer[0], READ_LIMIT); // Potentially change to use ssize_t val returned by read.
    std::string message_received(buffer.begin(), buffer.end());

    std::cout << "Message from client: " << message_received << std::endl;

    std::string message_sent("hello from the serverside\n");

    client.write(message_sent);
}

int main(void) {
    TcpListener listener = TcpListener::bind("localhost:4246").expect("Error binding TcpListener");

    std::cout << "Server running, listening to connections" << std::endl;
    while (true) {
        std::vector<TcpStream> clients = listener.select().unwrap();

        std::cout << "Clients received: " << clients.size() << std::endl;
        for (client_it client = clients.begin(); client != clients.end(); client++) {
            handle_client(*client);
        }
    }

    return 0;
}
