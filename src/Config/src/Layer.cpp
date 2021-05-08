#include "Layer.hpp"

// Layer Iterator - uses an identifier to iterate through child blocks that have
// the specified name

Layer::Iterator::Iterator(std::vector<Layer>& vec, std::string name)
    : it(vec.begin()), end(vec.end()), identifier(name) {}

Layer::Iterator::~Iterator(void) {}

Layer::Iterator::Iterator(Iterator const& src)
    : it(src.it), end(src.end), identifier(src.identifier) {}

Layer::Iterator& Layer::Iterator::operator=(Iterator const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    it = rhs.it;
    end = rhs.end;
    identifier = rhs.identifier;
    return *this;
}

Layer::Iterator& Layer::Iterator::operator++(void) {
    ++it;
    while (it->uid.name != identifier && it != end)
        ++it;
    return *this;
}

Layer::Iterator Layer::Iterator::operator++(int) {
    Iterator tmp(*this);
    operator++();
    return tmp;
}

Layer& Layer::Iterator::operator*() { return *it; }

Layer* Layer::Iterator::operator->() { return &(*it); }

bool Layer::Iterator::operator==(Iterator const& rhs) { return it == rhs.it; }

bool Layer::Iterator::operator!=(Iterator const& rhs) { return !(*this == rhs); }

// Layer Class

Layer::Layer(UID layer_uid) : uid(layer_uid) {}

Layer::Layer(std::string new_layer_name, uint8_t new_layer_depth, uint8_t new_layer_id_num)
    : uid(), parent(NULL) {
    uid.name = new_layer_name;
    uid.depth = new_layer_depth;
    uid.id_num = new_layer_id_num;
}

Layer::~Layer(void) {}

Layer& Layer::operator=(Layer const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    uid = rhs.uid;
    values = rhs.values;
    children = rhs.children;
    parent = rhs.parent;
    return *this;
}

Layer Layer::init(std::string layer_name, uint8_t layer_depth, uint8_t layer_id_num) {
    return Layer(layer_name, layer_depth, layer_id_num);
}

Layer::PushResult Layer::push_layer(std::string name) {
    uint8_t id = static_cast<uint8_t>(children.size());
    return push_layer(Layer::init(name, uid.depth + 1, id));
}

Layer::PushResult Layer::push_layer(Layer new_layer) {
    new_layer.parent = this;
    new_layer.uid.depth = uid.depth + 1;
    if (new_layer.uid.depth >= LAYER_DEPTH_LIMIT) {
        return PushResult::Err("Attempt to add layer beyond depth limit");
    }
    new_layer.uid.id_num = static_cast<uint8_t>(children.size());
    children.push_back(new_layer);
    return PushResult::Ok(&children.back());
}

Layer::Location Layer::get_location(void) const {
    Layer const* current = this;
    Location location;

    location.depth = current->uid.depth;
    while (current != NULL) {
        location.id_at_depth[current->uid.depth] = current->uid.id_num;
        current = current->parent;
    }
    return location;
}

void Layer::add_value(std::string key, std::string value) {
    values.insert(std::make_pair(key, value));
}

void Layer::add_value(std::pair<std::string, std::string> value) { values.insert(value); }

Layer::value_iterator Layer::begin_values(void) { return values.begin(); }

Layer::value_iterator Layer::end_values(void) { return values.end(); }

Layer::layer_iterator Layer::begin_children(void) { return children.begin(); }

Layer::layer_iterator Layer::end_children(void) { return children.end(); }

std::ostream& Layer::indent(std::ostream& o, size_t spaces) {
    for (size_t i = 0; i < spaces; i++)
        o << " ";
    return o;
}

std::ostream& operator<<(std::ostream& o, Layer::Location const& location) {
    o << "[";
    for (uint8_t i = 0; i <= location.depth; i++) {
        o << static_cast<unsigned int>(location.id_at_depth[i]);
        if (i != location.depth)
            o << ", ";
    }
    o << "]";
    return o;
}

std::string Layer::to_string(void) const {
    std::string output;

    output.append(static_cast<size_t>(uid.depth * 2), ' ');
    output.append(uid.name).append(" {\n");
    for (Layer::const_value_iterator it = values.begin(); it != values.end(); it++) {
        output.append(static_cast<size_t>(uid.depth * 2 + 2), ' ');
        output.append(it->first).append(" ").append(it->second).append(";\n");
    }
    output.push_back('\n');
    for (Layer::const_layer_iterator it = children.begin(); it != children.end(); it++) {
        output.append(it->to_string());
    }
    output.append(static_cast<size_t>(uid.depth * 2), ' ');
    output.append("}\n");

    return output;
}

std::ostream& operator<<(std::ostream& o, Layer const& layer) {
    layer.indent(o, layer.uid.depth * 2);
    o << layer.uid.name << " {\n";
    for (Layer::const_value_iterator it = layer.values.begin(); it != layer.values.end(); it++) {
        layer.indent(o, layer.uid.depth * 2 + 2);
        o << it->first << " " << it->second << ";\n";
    }
    o << "\n";
    for (Layer::const_layer_iterator it = layer.children.begin(); it != layer.children.end();
         it++) {
        o << *it;
    }
    layer.indent(o, layer.uid.depth * 2);
    o << "}" << std::endl;

    return o;
}