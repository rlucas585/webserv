#include "Parser.hpp"

#include "../../Utils/src/Utils.hpp"

namespace config {

bool ERROR = true;
bool SUCCESS = false;

Parser::Parser(void)
    : root_layer(Layer::init("global", 0, 0)), state(DIRECTIVE), prev_state(DIRECTIVE), line_num(1),
      error_msg("") {
    active_layer = &root_layer;
}

Parser::~Parser(void) {}

Parser::Parser(Parser const& src)
    : root_layer(src.root_layer), state(DIRECTIVE), prev_state(DIRECTIVE), line_num(1),
      error_msg("") {
    active_layer = &root_layer;
}

Parser& Parser::operator=(Parser const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    root_layer = rhs.root_layer;
    active_layer = &root_layer;
    state = rhs.state;
    prev_state = rhs.prev_state;
    directive_data = rhs.directive_data;
    value_data = rhs.value_data;
    line_num = rhs.line_num;
    error_msg = rhs.error_msg;
    return *this;
}

Parser::Result Parser::generate_config_from_file(const char* config_file) {
    File::Result file_res = File::open(config_file, O_RDONLY, 0644);

    if (file_res.is_err())
        return Parser::Result::Err(file_res.unwrap_err());

    BufReader<File> reader = BufReader<File>::init(file_res.unwrap());

    while (!reader.eof()) {
        if (parse_char(reader)) { // ERROR
            error_msg.append(Utils::to_string(line_num));
            return Parser::Result::Err(error_msg);
        }
    }
    return Parser::Result::Ok(root_layer);
}

bool Parser::parse_char(BufReader<File>& reader) {
    char c;
    Utils::RwResult res = reader.read_char(c);

    if (res.is_err()) {
        return set_error(res.unwrap_err().c_str());
    }

    if (c == '\n')
        line_num += 1;

    switch (state) {
    case DIRECTIVE:
        return parse_directive(c);
    case COMMENT:
        return parse_comment(c);
    case VALUE:
        return parse_value(c);
    default:
        return ERROR;
    }
}

bool Parser::parse_directive(char c) {
    if (Utils::isspace(c)) {
        if (directive_data.size() == 0)
            return false; // Ignore whitespace before directives
        state = VALUE;
    } else if (c == '{') {
        return add_new_layer();
    } else if (c == '}') {
        if (directive_data.size() != 0)
            return set_error("Expected value or \'{\', found \'}\'");
        if (active_layer->get_parent() == 0)
            return set_error(
                "Unexpected \'}\'"); // Attempt to exit scope with no parent scope available
        active_layer = active_layer->get_parent();
    } else if (c == ';') {
        if (directive_data.size() != 0)
            return set_error("\';\' found midway through directive");
    } else if (c == '#') {
        state = COMMENT;
    } else {
        directive_data.push_back(c);
    }
    return SUCCESS;
}

bool Parser::parse_value(char c) {
    if (Utils::isspace(c)) {
        if (value_data.size() == 0)
            return SUCCESS; // Ignore whitespace before value
        if (*value_data.rbegin() ==
            ' ') // space separates values of value_data, ignore multiple whitespace
            return SUCCESS;
        value_data.push_back(' ');
        return SUCCESS;
    } else if (c == '{') {
        return add_new_layer();
    } else if (c == '}') {
        return set_error("Unexpected \'}\'");
    } else if (c == ';') {
        if (value_data.size() == 0)
            return set_error("Expected value, found ';'"); // ';' found without any value
        if (directive_data == "include")
            return read_internal_file(value_data.c_str());
        active_layer->add_value(directive_data, value_data);
        directive_data.clear();
        value_data.clear();
        state = DIRECTIVE;
    } else if (c == '#') {
        state = COMMENT;
    } else {
        value_data.push_back(c);
    }
    return SUCCESS;
}

bool Parser::parse_comment(char c) {
    if (c == '\n') {
        state = prev_state; // Coments end at newline
    }
    return SUCCESS;
}

bool Parser::add_new_layer(void) {
    if (directive_data.size() == 0)
        return set_error("Invalid \'{\'");
    if (value_data.size() > 0)
        return set_error("Invalid \'{\'");
    Layer::Result new_layer_res = active_layer->push_layer(directive_data);
    if (new_layer_res.is_err())
        return set_error(new_layer_res.unwrap_err());
    directive_data.clear();
    active_layer = new_layer_res.unwrap();
    state = DIRECTIVE;
    return SUCCESS; // New scope was entered
}

bool Parser::set_error(const char* msg) {
    error_msg = "Error: ";
    error_msg += msg;
    error_msg += " at line ";
    return ERROR;
}

bool Parser::read_internal_file(const char* include_name) {
    Parser child_parser;
    Result child_file_res = child_parser.generate_config_from_file(include_name);

    if (child_file_res.is_err()) {
        return set_error(child_file_res.unwrap_err().c_str());
    }
    Layer child_global = child_file_res.unwrap();

    // Add all scopes of include file into main scope data structure
    for (Layer::Iterator child = child_global.begin_children();
         child != child_global.end_children(); child++) {
        Layer::Result push_result = active_layer->push_layer(*child);
        if (push_result.is_err()) {
            return set_error(push_result.unwrap_err());
        }
    }

    // Add all map values of include file into main scope data structure
    for (std::map<std::string, std::string>::iterator value = child_global.begin_values();
         value != child_global.end_values(); value++) {
        active_layer->add_value(value->first, value->second);
    }

    directive_data.clear();
    value_data.clear();
    state = DIRECTIVE;
    return SUCCESS;
}

} // namespace config
