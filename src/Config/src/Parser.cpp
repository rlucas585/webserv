#include "Parser.hpp"

#include "../../Utils/src/Utils.hpp"

namespace config {

bool ERROR = true;
bool SUCCESS = false;
bool IS_BLOCK = true;
bool IS_KEY = false;

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
        if (validate_block_directive())
            return ERROR;
        return add_new_layer();
    } else if (c == '}') {
        return set_error("Unexpected \'}\'");
    } else if (c == ';') {
        if (validate_key_directive())
            return ERROR;
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
    Layer::Result new_layer_res = active_layer->push_layer(directive_data);
    if (new_layer_res.is_err())
        return set_error(new_layer_res.unwrap_err());
    directive_data.clear();
    active_layer = new_layer_res.unwrap();
    if (value_data.size() > 0) {
        active_layer->add_value("Block-Information",
                                value_data); // Add additional info preceding '{'
        value_data.clear();
    }
    state = DIRECTIVE;
    return SUCCESS; // New scope was entered
}

bool Parser::set_error(const char* msg) {
    error_msg = "Error: ";
    error_msg += msg;
    error_msg += " at line ";
    return ERROR;
}

bool Parser::set_error(std::string msg) {
    error_msg = "Error: ";
    error_msg += msg;
    error_msg += " at line ";
    return ERROR;
}

bool Parser::validate_block_directive(void) {
    if (directive_data.size() == 0)
        return set_error("Invalid \'{\'");
    std::map<const Slice, Validator>::const_iterator search = valid_values.find(directive_data);

    if (search == valid_values.end())
        return set_error("Invalid directive: " + directive_data);

    Validator const& validator = search->second;
    if (!validator.is_block)
        return set_error("Invalid directive for block: " + directive_data);

    size_t argc = Slice(value_data).split().count_remaining();
    if (!validator.valid_value_range.contains(argc))
        return set_error("Invalid number of values for block: " + directive_data);

    return SUCCESS;
}

bool Parser::validate_key_directive(void) {
    if (value_data.size() == 0)
        return set_error("Expected value for " + directive_data + ", found ';'");

    if (directive_data == "include")
        return read_internal_file(value_data.c_str());

    std::map<const Slice, Validator>::const_iterator search = valid_values.find(directive_data);

    if (search == valid_values.end())
        return set_error("Invalid directive: " + directive_data);

    Validator const& validator = search->second;
    if (validator.is_block)
        return set_error(directive_data + " is a block value (requiring {}), not a key value");

    size_t argc = Slice(value_data).split().count_remaining();
    if (!validator.valid_value_range.contains(argc))
        return set_error("Invalid number of values for key: " + directive_data);

    return SUCCESS;
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

// Range helper struct - helps validating if number of values for a directive
// is correct

Parser::Range::Range(void) : min(Utils::nullopt), max(Utils::nullopt) {}

Parser::Range::Range(Utils::optional<size_t> min_val, Utils::optional<size_t> max_val)
    : min(min_val), max(max_val) {}

Parser::Range Parser::Range::init(Utils::optional<size_t> min_val,
                                  Utils::optional<size_t> max_val) {
    return Range(min_val, max_val);
}

Parser::Range& Parser::Range::set_min(size_t new_min) {
    min = Utils::make_optional(new_min);
    return *this;
}

Parser::Range& Parser::Range::set_max(size_t new_max) {
    max = Utils::make_optional(new_max);
    return *this;
}

bool Parser::Range::contains(size_t val) const {
    if (min.has_value() && val < *min)
        return false;
    if (max.has_value() && val > *max)
        return false;
    return true;
}

// Validator helper struct - To check if a config value is valid or not.

Parser::Validator::Validator(void) : is_block(false), valid_value_range() {}

Parser::Validator::Validator(bool is_block_value, Range num_values)
    : is_block(is_block_value), valid_value_range(num_values) {}

Parser::Validator Parser::Validator::init(bool is_block_value, Range num_values) {
    return Validator(is_block_value, num_values);
}

// Parser's valid values map contains information to validate whether a directive/value is
// valid, given its number of values and whether it is a block or a key.
// There is potential to add further validation in future by use of an optional predicate
// function pointer - but this is not currently implemented.
std::map<const Slice, Parser::Validator> Parser::create_valid_values_map(void) {
    std::map<const Slice, Validator> m;
    m["http"] = Validator::init(IS_BLOCK, Range().set_max(0));
    m["listen"] = Validator::init(IS_KEY, Range().set_min(1).set_max(2));
    m["server"] = Validator::init(IS_BLOCK, Range().set_max(0));
    m["root"] = Validator::init(IS_KEY, Range().set_min(1).set_max(1));
    m["server_name"] = Validator::init(IS_KEY, Range().set_min(1));
    m["location"] = Validator::init(IS_BLOCK, Range().set_min(1));
    m["client_max_body_size"] = Validator::init(IS_KEY, Range().set_min(1).set_max(1));
    m["limit_except"] = Validator::init(IS_BLOCK, Range().set_min(1));
    m["allow"] = Validator::init(IS_KEY, Range().set_min(1));
    m["deny"] = Validator::init(IS_KEY, Range().set_min(1));
    m["index"] = Validator::init(IS_KEY, Range().set_min(1));
    m["autoindex"] = Validator::init(IS_KEY, Range().set_min(1).set_max(1));

    return m;
}

const std::map<const Slice, Parser::Validator> Parser::valid_values =
    Parser::create_valid_values_map();

} // namespace config
