#include "../src/Tree.hpp"
#include <cstdio>
#include <gtest/gtest.h>
#include <string>

using namespace cfg;

TEST(Tree_tests, basics) {
    {
        Tree<std::string, std::string> tree;

        tree.insert("key", "value");

        tree.insert("key", "key1", "value");
        tree.insert("key1", "key11", "value");
        tree.insert("key1", "key12", "value");

        tree.insert("key", "key2", "value");
        tree.insert("key2", "key21", "value");
        tree.insert("key2", "key22", "value");
    }
}

TEST(Tree_tests, find) {
    {
        typedef std::string K;
        typedef std::string V;
        typedef Tree<K, V> Tree;
        Tree tree;

        tree.insert("key", "value");

        tree.insert("key", "key1", "value1");
        tree.insert("key1", "key11", "value11");
        tree.insert("key1", "key12", "value12");

        tree.insert("key", "key2", "value2");
        tree.insert("key2", "key21", "value21");
        tree.insert("key2", "key22", "value22");

        Tree::iterator iter;

        iter = tree.find("key1");
        EXPECT_EQ((*iter)->get_value(), "value1");

        iter = tree.find("key11");
        EXPECT_EQ((*iter)->get_value(), "value11");

        iter = tree.find("key22");
        EXPECT_EQ((*iter)->get_value(), "value22");
    }
}
