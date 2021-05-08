#include "Parser.hpp"

#include "../../Utils/src/Utils.hpp"

namespace config {

Parser::Parser(void) : root_layer(Layer::init("global", 0, 0)), state(GLOBAL) {
    active_layer = &root_layer;
}

Parser::~Parser(void) {}

Parser::Parser(Parser const& src) : root_layer(src.root_layer), state(GLOBAL) {
    active_layer = &root_layer;
}

Parser& Parser::operator=(Parser const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    root_layer = rhs.root_layer;
    active_layer = &root_layer;
    state = rhs.state;
    return *this;
}

Parser::Result Parser::generate_config_from_file(const char* config_file) {
    File::Result file_res = File::open(config_file, O_RDONLY, 0644);
    std::string error_msg("Error: Invalid config file at line ");
    size_t count = 0;

    if (file_res.is_err())
        return Parser::Result::Err(file_res.unwrap_err());

    BufReader<File> reader = BufReader<File>::init(file_res.unwrap());

    while (!reader.eof()) {
        count += 1;
        if (parse_line(reader)) { // Error
            error_msg.append(Utils::to_string(count));
            return Parser::Result::Err(error_msg);
        }
    }
    return Parser::Result::Ok(root_layer);
}

bool Parser::parse_line(BufReader<File>& reader) {
    std::string line;
    std::vector<Slice> slices;
    Slice without_comment;

    reader.read_line(line);
    without_comment = Slice(line).trim();
    if (without_comment.begin() == without_comment.end() || *without_comment.begin() == '#')
        return false; // Line is empty, or commented out
    without_comment = without_comment.split("#").next();
    std::cout << without_comment << std::endl;

    slices = without_comment.split().collect<std::vector<Slice> >();
    // for (std::vector<Slice>::const_iterator it = slices.begin(); it != slices.end(); it++)
    //   std::cout << *it << std::endl;

    return false;
}

} // namespace config
