/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TcpListener.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 21:57:46 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/05 00:53:27 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
    typedef Utils::result<std::vector<TcpStream>, std::string> SelectResult;

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

    AcceptResult accept(void) const;
    SelectResult select(void);

    Utils::result<int, std::string> accept_raw(void) const;

    bool operator==(TcpListener const& other) const;
    bool operator!=(TcpListener const& other) const;

  private:
    Socket inner;
    SelectConfig config;

    void accept_new_client(void);
};

#endif
