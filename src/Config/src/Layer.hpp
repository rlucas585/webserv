#ifndef LAYER_HPP
#define LAYER_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../../Option/src/optional.hpp"
#include "../../Result/src/result.hpp"
#include "../../Slice/src/Slice.hpp"
#include "../../Sys/src/BufReader.hpp"
#include "../../Sys/src/File.hpp"

#define LAYER_DEPTH_LIMIT 5

typedef unsigned char uint8_t;

// Layer class - contains a configuration like data structure

class Layer {
  public:
    struct Location {
        uint8_t depth;
        uint8_t id_at_depth[LAYER_DEPTH_LIMIT];

        bool operator==(Location const& rhs);

        size_t last_common_depth(Location const& rhs);
    };

    struct UID {
        std::string name;
        uint8_t depth;
        uint8_t id_num;
    };

  public:
    class Iterator {
      public:
        Iterator(std::vector<Layer>& vec);
        Iterator(std::vector<Layer>& vec, std::string name);
        Iterator(std::vector<Layer>::iterator new_it);
        ~Iterator(void);
        Iterator(Iterator const& src);
        Iterator& operator=(Iterator const& rhs);

        Iterator& operator++(void);
        Iterator operator++(int);

        Layer& operator*();
        Layer* operator->();

        bool operator==(Iterator const& rhs);
        bool operator!=(Iterator const& rhs);

      private:
        std::vector<Layer>::iterator it;
        std::vector<Layer>::iterator end;
        Utils::optional<std::string> identifier;

        Iterator(void);
    };

  public:
    typedef Utils::result<Layer*, const char*> Result;
    typedef Utils::result<Location, const char*> LocationRes;

  public:
    typedef std::map<std::string, std::string>::iterator value_iterator;
    typedef std::map<std::string, std::string>::const_iterator const_value_iterator;
    typedef std::vector<Layer>::iterator layer_iterator;
    typedef std::vector<Layer>::const_iterator const_layer_iterator;

  public:
    ~Layer(void);
    Layer(UID layer_uid);
    Layer(Layer const& src)
        : uid(src.uid), values(src.values), children(src.children), parent(src.parent) {}
    Layer& operator=(Layer const& rhs);

    static Layer init(std::string layer_name, uint8_t layer_depth, uint8_t layer_id_num);

    Result push_layer(std::string name);
    Result push_layer(Layer new_layer);
    Result get_layer(Location location);
    Location get_location(void) const;

    void add_value(std::string key, std::string value);
    void add_value(std::pair<std::string, std::string> value);

    std::string const& get_name(void) const;
    Utils::optional<std::string*> get_value(Slice key);

    value_iterator begin_values(void);
    value_iterator end_values(void);
    Iterator begin_children(void);
    Iterator end_children(void);
    Iterator filter_children(std::string filter);

    Layer* get_parent(void);

    std::string to_string(void) const;

    friend std::ostream& operator<<(std::ostream& o, Layer const& layer);

  private:
    UID uid;
    std::map<std::string, std::string> values;
    std::vector<Layer> children;
    Layer* parent;

    Layer(void);
    Layer(std::string layer_name, uint8_t layer_depth, uint8_t layer_id_num);

    Result increase_depth(void);
    static std::ostream& indent(std::ostream& o, size_t spaces);
};

std::ostream& operator<<(std::ostream& o, Layer::Location const& location);
std::ostream& operator<<(std::ostream& o, Layer const& layer);

#endif
