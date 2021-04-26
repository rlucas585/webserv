#include "Server.hpp"

#include <cerrno>
#include <cstring>

Server::SelectConfig::SelectConfig(void) {}

Server::SelectConfig::~SelectConfig(void) {}

Server::SelectConfig::SelectConfig(SelectConfig const& other) { *this = other; }

Server::SelectConfig& Server::SelectConfig::operator=(SelectConfig const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    current_sockets = rhs.current_sockets;
    max_fd = rhs.max_fd;
    return *this;
}

Server::Server(std::vector<TcpListener>& tcp_listeners)
    : listeners(tcp_listeners), config(), clients() {
    FD_ZERO(&config.current_sockets);
    for (size_t i = 0; i < listeners.size(); i++) {
        FD_SET(listeners[i].socket_fd(), &config.current_sockets);
    }
    config.max_fd = listeners.back().socket_fd();
    // Default construct all Clients
    for (size_t i = 0; i < CLIENT_TOTAL - listeners.size(); i++) {
        clients.push_back(Client());
    }
}

Server::~Server(void) {}

Server::Server(Server const& other)
    : listeners(other.listeners), config(other.config), clients(other.clients) {}

Server& Server::operator=(Server const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    listeners = rhs.listeners;
    config = rhs.config;
    clients = rhs.clients;
    return *this;
}

// Create a Server with a single TcpListener
Server::Result Server::bind(const char* str) {
    SocketAddrV4::Result addr = SocketAddrV4::init(str);

    if (addr.is_err()) {
        return Server::Result::Err(addr.unwrap_err());
    }
    TcpListener listener = TcpListener::bind(addr.unwrap()).unwrap();
    std::vector<TcpListener> listeners;

    listeners.push_back(listener);
    return Server::Result::Ok(Server(listeners));
}

Server Server::init(std::vector<TcpListener> tcp_listeners) { return Server(tcp_listeners); }

Server::SelectResult Server::select(std::vector<Client*>& output_clients) {
    output_clients.clear();

    // ::select() is destructive - copies required
    fd_set read_sockets = config.current_sockets;
    fd_set write_sockets = config.current_sockets;

    if (::select(config.max_fd + 1, &read_sockets, &write_sockets, 0, 0) < 0) {
        return Server::SelectResult::Err(strerror(errno));
    }

    for (int fd = 0; fd < config.max_fd + 1; fd++) {
        if (FD_ISSET(fd, &read_sockets)) {
            if (incoming_client(fd)) {
                accept_new_client(fd);
            } else {
                process_read_client(fd, output_clients);
            }
        } else if (FD_ISSET(fd, &write_sockets)) {
            // TODO Complete to handle clients that are ready to receive data
            if (fd <= 2) {
                break;
            }
            // size_t client_index = client_fd_to_index(fd);
            // clients[client_index].state = Client::Write;
            // output_clients.push_back(&clients[client_index]);
        }
    }

    return Server::SelectResult::Ok(0);
}

Server::AcceptResult Server::accept_new_client(int listener_fd) {
    // Get reference to relevant TcpListener
    Server::AcceptResult listener_res = get_listener(listener_fd);
    if (listener_res.is_err()) {
        return listener_res; // This "should" be an impossible error
    }
    TcpListener& listener = *listener_res.unwrap();

    // Call accept() on relevant listener
    Utils::result<int, std::string> res = listener.accept_raw();

    if (res.is_ok()) {
        int new_fd = res.unwrap();

        if (new_fd > config.max_fd) {
            config.max_fd = new_fd;
        }
        FD_SET(new_fd, &config.current_sockets);

        // Set backing store client as CONNECTED and create TcpStream
        clients[client_fd_to_index(new_fd)].activate_client(new_fd);
    } else {
        // print error to stderr and continue, (no need to crash for one failed client)
        std::cerr << "accept_raw() error: " << res.unwrap_err() << std::endl;
    }
    return Server::AcceptResult::Ok(0); // Return value unused, would be () if in C++
}

void Server::process_read_client(int client_fd, std::vector<Client*>& output_clients) {
    char c;
    ssize_t peek_result = ::recv(client_fd, &c, 1, MSG_PEEK);
    size_t client_index = client_fd_to_index(client_fd);

    if (peek_result == 0) { // Client disconnected - Deactivate and remove from fd_set
        FD_CLR(client_fd, &config.current_sockets);
        clients[client_index].deactivate_client();
    } else { // Client has sent information, set as ready to Read, and add to output_clients
        clients[client_index].state = Client::Read;
        output_clients.push_back(&clients[client_index]);
    }
}

bool Server::incoming_client(int fd) {
    if (fd <= listeners.back().socket_fd())
        return true;
    return false;
}

Server::AcceptResult Server::get_listener(int listener_fd) {
    for (size_t i = 0; i < listeners.size(); i++) {
        if (listeners[i].socket_fd() == listener_fd)
            return Server::AcceptResult::Ok(&listeners[i]);
    }
    return Server::AcceptResult::Err("TcpListener not found");
}

int Server::client_fd_to_index(int fd) { return fd - 3 - listeners.size(); }

int Server::client_index_to_fd(int index) { return index + 3 + listeners.size(); }
