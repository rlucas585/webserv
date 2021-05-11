#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include "../../Config/src/Layer.hpp"
#include "../../Net/src/TcpListener.hpp"

class VirtualServer {
  public:
    typedef Utils::result<VirtualServer, std::string> Result;
    typedef Utils::result<Utils::pair<TcpStream, SocketAddrV4>, std::string> AcceptResult;

  public:
    VirtualServer(void);
    ~VirtualServer(void);
    VirtualServer(VirtualServer const& other);
    VirtualServer& operator=(VirtualServer const& rhs);

    static Result bind(Layer* server_configuration);

    Socket const& socket(void) const;
    int socket_fd(void) const;
    AcceptResult accept(void) const;
    Utils::result<int, std::string> accept_raw(void) const;

  private:
    SocketAddrV4 address;
    Layer const* config;
};

#endif
