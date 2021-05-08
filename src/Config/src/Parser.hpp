#ifndef PARSER_HPP
#define PARSER_HPP

#include "../../Result/src/result.hpp"
#include "../../Slice/src/Slice.hpp"
#include "../../Sys/src/BufReader.hpp"
#include "../../Sys/src/File.hpp"
#include "Layer.hpp"

namespace config {

class Parser {
  private:
    enum State {
        GLOBAL,
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
    Layer root_layer;
    Layer* active_layer;
    State state;

  private:
    bool parse_line(BufReader<File>& reader);
};

} // namespace config

#endif
