#include "Client.hpp"

Client::Client(void) : state(Client::Inactive), stream(), parser(), config(0) {}
// Client::Client(void) : state(Client::Inactive), stream(), parser(), config(0), address() {}

Client::~Client(void) {}

Client::Client(Client const& other) { *this = other; }

Client& Client::operator=(Client const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    state = rhs.state;
    stream = rhs.stream; // Move semantics
    parser = rhs.parser;
    config = rhs.config;
    stored_data = rhs.stored_data;
    // address = rhs.address;
    return *this;
}

void Client::activate_client(int fd) {
    stream = TcpStream::init_from_socket(Socket::init_from_raw(fd));
    state = Client::Connected;
}

void Client::deactivate_client(void) {
    // Overwriting the internal TcpStream will implicitly call close on the
    // file descriptor
    stream = TcpStream();
    state = Client::Inactive;
}

TcpStream& Client::get_stream(void) { return stream; }

int Client::fd(void) const { return stream.fd(); }

Utils::RwResult Client::read(void) {
    char buf[READ_AMOUNT];

    if (state != Read) {
        return Utils::RwResult::Err("read called on client not ready to read");
    }

    Utils::RwResult read_result = stream.read(reinterpret_cast<void*>(buf), READ_AMOUNT);

    if (read_result.is_ok()) {
        size_t bytes_read = read_result.unwrap();

        if (bytes_read == 0) {
            state = Close; // Client sends 0 bytes to signal connection closure
            return Utils::RwResult::Ok(bytes_read);
        }
        Slice buf_slice = Slice::newSliceWithLength(buf, bytes_read);
        while (buf_slice.length() != 0) {
            Utils::optional<Slice> next_opt = buf_slice.strchr('\n');

            if (!next_opt.has_value()) {
                // If no value, append remaining characters to Client storage and return
                stored_data.append(buf_slice.raw(), buf_slice.length());
                state = Connected;
                return Utils::RwResult::Ok(bytes_read);
            }
            Slice next = next_opt.unwrap();
            size_t length = next.raw() - buf_slice.raw();
            Slice current_line = Slice::newSliceWithLength(buf_slice.raw(), length + 1);

            if (stored_data.length() != 0) {
                // If we have unparsed data from a previous read, then append new data to
                // it before parsing the stored_data, then clear
                stored_data.append(current_line.raw(), current_line.length());
                parser.parse_line(stored_data);
                stored_data.clear();
            } else {
                parser.parse_line(current_line);
            }

            buf_slice = Slice::newSliceWithOffset(next, 1); // Move buf_slice past \n
        }
    }
    state = Connected;
    return read_result;
}

Utils::RwResult Client::write(std::string const& str) { return stream.write(str); }

bool Client::request_is_complete(void) const { return parser.is_complete(); }

http::Request::Result Client::generate_request(void) {
    // If there is remaining unread data, read all into the parser
    if (stored_data.length() != 0) {
        parser.parse_line(stored_data);
        stored_data.clear();
    }
    return parser.generate_request();
}

void Client::set_address(SocketAddrV4 new_address) {
  address = new_address;
}
