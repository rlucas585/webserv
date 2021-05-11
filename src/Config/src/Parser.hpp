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

extern bool IS_BLOCK;
extern bool IS_KEY;

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

  private:
    struct Range {
        Utils::optional<size_t> min;
        Utils::optional<size_t> max;

        Range(void);
        Range(Utils::optional<size_t> min_val, Utils::optional<size_t> max_val);

        static Range init(Utils::optional<size_t> min_val, Utils::optional<size_t> max_val);

        Range& set_min(size_t new_min);
        Range& set_max(size_t new_max);

        bool contains(size_t val);
    };

    struct Validator {
        bool is_block;
        Range valid_value_range;
        // Utils::optional<bool (*)()> predicate; // Potential to add additional validation with
        // function

        Validator(void);
        Validator(bool is_block_value, Range valid_value_range);

        static Validator init(bool is_block_value, Range num_values);
    };

    static std::map<const Slice, Validator> create_valid_values_map(void);
    static const std::map<const Slice, Validator> valid_values;
};

} // namespace config

#endif
