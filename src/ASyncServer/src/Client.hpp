#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../../HTTP/src/Request.hpp"
#include "../../Net/src/TcpStream.hpp"
#include "../../Sys/src/BufReader.hpp"

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

    TcpStream& stream(void);
    int fd(void) const;

    Utils::RwResult read_line(std::string& buf);
    Utils::RwResult read_until(char delimiter, std::string& buf);

    Utils::RwResult write(std::string const& str);

    bool eof(void);

    bool parse_http(void);
    http::Request::Result generate_request(void);

  public:
    Client::Status state;

  private:
    BufReader<TcpStream> reader;
    http::Request::Parser parser;
};

#endif
