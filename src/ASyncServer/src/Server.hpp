#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../Net/src/TcpListener.hpp"
#include "../../Result/src/result.hpp"
#include "Client.hpp"

#include <sys/select.h>
#include <vector>

#define CLIENT_TOTAL 1000

class Server {
  public:
    typedef Utils::result<Server, std::string> Result;
    typedef Utils::result<size_t, std::string> SelectResult;
    typedef Utils::result<TcpListener*, std::string> AcceptResult;

  private:
    struct SelectConfig {
        SelectConfig(void);
        ~SelectConfig(void);
        SelectConfig(SelectConfig const& other);
        SelectConfig& operator=(SelectConfig const& rhs);

        fd_set current_sockets;
        int max_fd;
    };

  public:
    ~Server(void);
    Server(Server const& other);
    Server& operator=(Server const& rhs);

    static Result bind(const char* str);
    static Server init(std::vector<TcpListener> tcp_listeners);

    SelectResult select(std::vector<Client*>& clients);

  private:
    std::vector<TcpListener> listeners;
    SelectConfig config;
    std::vector<Client> clients;

    Server(void);
    Server(std::vector<TcpListener>& tcp_listeners);

    AcceptResult accept_new_client(int listener_fd);
    void process_read_client(int client_fd, std::vector<Client*>& output_clients);

    bool incoming_client(int fd);
    AcceptResult get_listener(int listener_fd);
    int client_fd_to_index(int fd);
    int client_index_to_fd(int fd);
};

#endif
