#include "Client.hpp"

Client::Client(void) : state(Client::Inactive), reader(), parser() {}

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

Utils::RwResult Client::read_line(std::string& buf) {
    // If our Client has been triggered to Read, then the internal BufReader should consume the rest
    // of it's contents, then continue reading
    if (state == Read) {
        reader.read_remaining(buf);
        reader.fill_buf();
        state = Processing;
    }
    return reader.read_line(buf);
}

Utils::RwResult Client::read_until(char delimiter, std::string& buf) {
    if (state == Read) {
        reader.read_remaining(buf);
        reader.fill_buf();
        state = Processing;
    }
    return reader.read_until(delimiter, buf);
}

Utils::RwResult Client::write(std::string const& str) { return reader.as_inner().write(str); }

bool Client::eof(void) {
    if (state == Read) {
        return false;
    }
    return reader.eof();
}

bool Client::parse_http(void) {
    std::string line;
    size_t lines_read = 0;

    while (!eof()) {
        Utils::RwResult read_result = read_line(line);
        if (read_result.is_err()) {
            // TODO indicates failure of ::read. Should be unlikely, but if it does occur
            // then 500 Internal Server Error should be set as Response
        } else {
            size_t bytes_read = read_result.unwrap();
            if (bytes_read == 0)
                break;
            parser.parse_line(Slice(line));
            line.clear();
            lines_read += 1;
        }

        if (lines_read == 0) {
            state = Close; // If select is triggered with 0 bytes to send, the client has closed
                           // connection
            break;
        }
    }
    return parser.is_complete();
}

http::Request::Result Client::generate_request(void) {
    // If the parser is still waiting to read a body, this function will read all
    // remaining data.
    std::string line;

    while (parser.ready_for_body().has_value()) {
        size_t amount_left = parser.ready_for_body().unwrap();

        if (amount_left == 0)
            break;
        reader.read_remaining(line);
        parser.parse_line(Slice(line));
        if (line.size() == 0) // If there is simply nothing left to read, then exit
            break;
        line.clear();
    }

    return parser.generate_request();
}

// if (parser.is_complete()) {
//   http::Request::Result req_res = parser.generate_request();
//   if (req_res.is_err()) {
//     std::cout << "Invalid request:" << std::endl;
//     std::cout << req_res.unwrap_err() << std::endl;
//   } else {
//     std::cout << "Valid request: " << std::endl;
//     std::cout << req_res.unwrap();
//   }
//   parser = http::Request::Parser();
// }
