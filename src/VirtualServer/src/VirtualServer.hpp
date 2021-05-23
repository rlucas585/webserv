#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include "../../Config/src/Layer.hpp"
#include "../../Net/src/TcpListener.hpp"

class VirtualServer {
  public:
    typedef Utils::result<VirtualServer, std::string> Result;
    typedef Utils::result<Utils::pair<TcpStream, SocketAddrV4>, std::string> AcceptResult;

  public:
    ~VirtualServer(void);
    VirtualServer(VirtualServer const& other);
    VirtualServer& operator=(VirtualServer const& rhs);

    static Result create(Layer const* server_configuration);

    SocketAddrV4 address(void) const;

  private:
    VirtualServer(void);
    VirtualServer(Layer const* server_configuration);

    Layer const* config;

    static bool validate_listen(Slice listen);

    static bool INVALID;
    static bool VALID;
};

#endif
