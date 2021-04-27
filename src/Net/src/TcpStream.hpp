#ifndef TCPSTREAM_HPP
#define TCPSTREAM_HPP

#include "../../Result/src/result.hpp"
#include "Socket.hpp"

class TcpStream {
  public:
    typedef bool stream_category;

  public:
    typedef Utils::result<TcpStream, std::string> Result;

  public:
    TcpStream(void);
    ~TcpStream(void);
    TcpStream(Socket sock);
    TcpStream(TcpStream const& other);
    TcpStream& operator=(TcpStream const& rhs);

    static TcpStream::Result connect(const char* str);
    static TcpStream::Result connect(SocketAddr const& addr);
    static TcpStream init_from_socket(Socket sock);

    Utils::RwResult read(void* buf, size_t len);
    Utils::RwResult read(std::string& str);
    Utils::RwResult peek(void);
    Utils::RwResult write(const void* buf, size_t len);
    Utils::RwResult write(const char* str);
    Utils::RwResult write(std::string const& str);

    int fd(void) const;

  private:
    Socket inner;

    static const size_t PEEK_LIMIT = 50;
};

#endif
