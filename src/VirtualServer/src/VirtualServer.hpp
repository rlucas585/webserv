#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include "Layer.hpp"
#include "TcpListener.hpp"

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
    Utils::optional<std::string const*> name(void) const;

    bool operator==(VirtualServer const& other);

  private:
    VirtualServer(void);
    VirtualServer(Layer const* server_configuration);

    Layer const* config;

    static bool validate_listen(Slice listen);

    static bool INVALID;
    static bool VALID;
};

#endif
