#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <iterator>

#include <algorithm>
#include <exception>
#include <vector>

#include "../../Utils/src/Utils.hpp"

// I LOVE C++98, it's a breeze to write and read it
// This is an aweful library, that should be shot at first daybreak

namespace cfg {

template <typename K, typename V>
class Tree;
template <typename T>
class TreeDFIterator;

template <typename K, typename V>
class Node {
  public:
    typedef K KeyType;
    typedef V ValueType;

  public:
    Node(const Node* parent) : parent(parent) {}
    Node(const Node* parent, KeyType& key) : parent(parent), key(key) {}
    ~Node() {
        for (typename std::vector<Node*>::iterator iter = children.begin(); iter != children.end();
             iter++) {
            delete *iter;
        }
    }
    void set_key(KeyType& key) { this->key = key; };
    void set_value(ValueType& value) { this->value = value; }
    ValueType get_value(void) { return this->value; }
    void push_child(Node* node) { children.push_back(node); }

    bool operator==(const Node& rhs) const { return (key == rhs.key); }
    bool operator!=(const Node& rhs) const { return (key != rhs.key); }

  private:
    const Node* parent; // Optional<> ?
    std::vector<Node*> children;
    KeyType key;
    ValueType value;
    friend class Tree<K, V>;
    friend class TreeDFIterator<Node<K, V> >;
};

template <typename T>
class TreeDFIterator {
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;

    TreeDFIterator() : _data(NULL) {}
    TreeDFIterator(pointer _data) : _data(_data) {}
    TreeDFIterator(reference _data) : _data(&_data) {}
    TreeDFIterator(value_type _data) : _data(&_data) {}
    ~TreeDFIterator(void) {}
    TreeDFIterator(TreeDFIterator const& other) { *this = other; }

    pointer operator*(void) { return _data; }

    TreeDFIterator operator+(size_t inc) {
        TreeDFIterator iter(this);
        for (; inc > 0; inc--) {
            iter++;
        }
        return iter;
    }

    bool operator==(const TreeDFIterator& rhs) const {
        return (_data == rhs._data ||
                (_data != NULL && rhs._data != NULL && _data->key == rhs._data->key));
    }

    bool operator!=(const TreeDFIterator& rhs) const { return !(rhs == *this); }
    TreeDFIterator& operator=(TreeDFIterator const& rhs) {
        _data = rhs._data;
        return *this;
    }

    TreeDFIterator operator++(int) {
        std::cerr << "next data:" << _data->key << std::endl;
        _data = next_data(_data);
        return (TreeDFIterator(next_data(_data)));
    }

    TreeDFIterator& operator++() {
        _data = next_data(_data);
        std::cerr << "next data:" << _data->key << std::endl;
        return *this;
    }

    pointer next_data(pointer data) {
        if (data->children.size() > 0) {
            std::cerr << "next_data: in middlenode, going deeper" << std::endl;
            data = data->children.front(); // go in depth first
        } else {
            std::cerr << "next_data: leafnode found, backtracking" << std::endl;

            pointer last_data = data;
            while ((data = const_cast<pointer>(data->parent))) {
                typename std::vector<pointer>::iterator const& iter =
                    std::find(data->children.begin(), data->children.end(), last_data);

                if (iter != data->children.end() && iter + 1 != data->children.end()) {
                    data = *(iter + 1);
                    break;
                }
                last_data = data;
            }
        }
        return data;
    }

  private:
    pointer _data;
};

template <typename K, typename V>
class Tree {
  public:
    typedef K KeyType;
    typedef V ValueType;
    typedef Node<KeyType, ValueType> NodeType;
    typedef TreeDFIterator<NodeType> iterator;

  public:
    Tree(void) : root(NULL) {}
    ~Tree(void) { delete root; }

    // insert a root key
    void insert(KeyType key, ValueType value) {
        if (root != NULL) {
            throw std::runtime_error("Root key is already set!");
            return;
        }
        root = new NodeType(NULL);
        root->set_key(key);
        root->set_value(value);
        return;
    }

    // insert for a given parent key
    void insert(KeyType parent_key, KeyType key, ValueType value) {
        if (root == NULL) {
            throw std::runtime_error("Root key is not set!");
            return;
        }
        iterator iter = this->find(parent_key);
        if (iter != this->end()) {
            NodeType* parent_node = *iter;
            if (parent_node->children.size() != 0) {
                for (typename std::vector<NodeType*>::iterator iter = parent_node->children.begin();
                     iter != parent_node->children.end(); iter++) {
                    if ((*iter)->key == key) {
                        throw std::runtime_error("Parent already has this key!");
                        return;
                    }
                }
            }
            Node<KeyType, ValueType>* new_node = new Node<KeyType, ValueType>(parent_node);
            new_node->set_key(key);
            new_node->set_value(value);
            parent_node->push_child(new_node);
        } else {
            throw std::runtime_error("Parent key does not exist in tree");
        }
    }

    iterator find(const KeyType& key) {
        iterator iter = this->begin();
        while (iter != this->end() && (*iter)->key != key)
            iter++;
        return iter;
    }

  public:
    iterator begin() { return (iterator(root)); }
    iterator end() { return (iterator()); }

  private:
    Node<KeyType, ValueType>* root;
};

} // namespace cfg

#endif
