#include "Client.hpp"

Client::Client(void) : state(Client::Inactive), reader(), data() {
    data.reserve(CLIENT_BUFFER_SIZE);
}

Client::~Client(void) {}

Client::Client(Client const& other) { *this = other; }

Client& Client::operator=(Client const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    reader = rhs.reader;
    return *this;
}

void Client::activate_client(int fd) {
    TcpStream new_stream = TcpStream::init_from_socket(Socket::init_from_raw(fd));
    reader.reset(new_stream);
    state = Client::Connected;
}

void Client::deactivate_client(void) {
    // Overwriting the internal TcpStream will implicitly call close on the
    // file descriptor
    reader.reset(TcpStream());
    state = Client::Inactive;
}

TcpStream& Client::stream(void) { return reader.as_inner(); }

int Client::fd(void) const { return reader.as_inner().fd(); }

Utils::RwResult Client::read_line(std::string& buf) { return reader.read_line(buf); }

Utils::RwResult Client::read_until(char delimiter, std::string& buf) {
    return reader.read_until(delimiter, buf);
}

Utils::RwResult Client::write(std::string const& str) { return reader.as_inner().write(str); }
