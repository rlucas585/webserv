// An example using the TcpStream, together with the HTTP::Request class,
// to send an HTTP Request to a website

// HTTP Requests are built using the Request::Builder class.
// They default to "GET / HTTP/1.1", but support additional methods.
// In future, all methods should be supported by the Builder class.

#include <iostream>

#include <netdb.h>
#include <stdlib.h>

#include "../../HTTP/src/Request.hpp"
#include "../../Net/src/TcpStream.hpp"
#include "../../Sys/src/BufReader.hpp"
#include "../../Utils/src/Utils.hpp"

struct addrinfo* parse_domain(char* domain, char* port) {
    struct addrinfo hints;
    struct addrinfo* servinfo;

    Utils::memset(&hints, '\0', sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(domain, port, &hints, &servinfo) != 0) {
        std::cerr << "Error in getaddrinfo " << std::endl;
        exit(1);
    }
    return servinfo;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << argv[0] << " requires 2 arguments, a host and port to connect to" << std::endl;
        return 1;
    }

    struct addrinfo* servinfo = parse_domain(argv[1], argv[2]);

    SocketAddrV4 socket_addr =
        SocketAddrV4::init(reinterpret_cast<sockaddr_in&>(*servinfo->ai_addr));

    freeaddrinfo(servinfo);

    TcpStream::Result server_res = TcpStream::connect(socket_addr);

    if (server_res.is_err()) {
        std::cerr << "Error: " << server_res.unwrap_err() << std::endl;
        return 1;
    }
    TcpStream server = server_res.unwrap();

    http::Request req = http::Request::Builder().header("Host", argv[1]).build();

    std::cout << "Request:\n" << req;

    server.write(req.to_string());

    BufReader<TcpStream> reader;

    reader.reset(server);

    std::string response;

    std::cout << "Response:\n";
    while (!reader.eof()) {
        reader.read_line(response);
        std::cout << response;
        response.clear();
    }

    return 0;
}
