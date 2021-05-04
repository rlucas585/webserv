#ifndef TCPLISTENER_HPP
#define TCPLISTENER_HPP

#include "../../Utils/src/pair.hpp"
#include "Socket.hpp"
#include "TcpStream.hpp"
#include <vector>

class TcpListener {
  public:
    typedef Utils::result<TcpListener, std::string> Result;
    typedef Utils::result<Utils::pair<TcpStream, SocketAddrV4>, std::string> AcceptResult;

  private:
    struct SelectConfig {
        SelectConfig(void);
        ~SelectConfig(void);

        fd_set current_sockets;
        int max_fd;
    };

  public:
    TcpListener(void);
    ~TcpListener(void);
    TcpListener(Socket sock);
    TcpListener(TcpListener const& other);
    TcpListener& operator=(TcpListener const& rhs);

    static Result bind(const char* str);
    static Result bind(SocketAddr const& addr);

    Socket const& socket(void) const;

    int socket_fd(void) const;

    AcceptResult accept(void) const;

    Utils::result<int, std::string> accept_raw(void) const;

    bool operator==(TcpListener const& other) const;
    bool operator!=(TcpListener const& other) const;

  private:
    Socket inner;
    SelectConfig config;
};

#endif
