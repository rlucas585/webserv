#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "BufReader.hpp"
#include "Layer.hpp"
#include "Request.hpp"
#include "TcpStream.hpp"

#define READ_AMOUNT 1024

class Client {
  public:
    enum Status { Connected, Read, Processing, RequestReady, Write, Close, Inactive };

  public:
    Client(void);
    ~Client(void);
    Client(Client const& other);
    Client& operator=(Client const& rhs);

    void activate_client(int fd);
    void deactivate_client(void);

    TcpStream& get_stream(void);
    int fd(void) const;

    Utils::RwResult read(void);

    Utils::RwResult write(std::string const& str);

    bool request_is_complete(void) const;
    http::Request::Result generate_request(void);

  public:
    Client::Status state;

  private:
    TcpStream stream;
    http::Request::Parser parser;
    Layer const* config;
    std::string stored_data;
};

#endif
