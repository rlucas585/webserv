#ifndef PARSER_HPP
#define PARSER_HPP

#include "../../Result/src/result.hpp"
#include "../../Slice/src/Slice.hpp"
#include "../../Sys/src/BufReader.hpp"
#include "../../Sys/src/File.hpp"
#include "Layer.hpp"

namespace config {

extern bool ERROR;
extern bool SUCCESS;

class Parser {
  private:
    enum State {
        DIRECTIVE,
        COMMENT,
        VALUE,
    };

  public:
    typedef Utils::result<Layer, std::string> Result;

  public:
    Parser(void);
    ~Parser(void);
    Parser(Parser const& src);
    Parser& operator=(Parser const& rhs);

    Result generate_config_from_file(const char* config_file);

  private:
    // Probably too many variables
    Layer root_layer;
    Layer* active_layer;
    State state;
    State prev_state; // Used for handling comments
    std::string directive_data;
    std::string value_data;
    size_t line_num;
    std::string error_msg;

  private:
    bool parse_char(BufReader<File>& reader);
    bool parse_directive(char c);
    bool parse_comment(char c);
    bool parse_value(char c);
    bool add_new_layer(void);
    bool set_error(const char* msg);

    bool read_internal_file(const char* include_name);
};

} // namespace config

#endif
