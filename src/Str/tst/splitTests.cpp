#include <gtest/gtest.h>

#include "../src/Str.hpp"

#include <iostream>
#include <vector>

TEST(StrSplit, split_test_basic) {
    Str str = "A few words";
    Str::Split iter = str.split();

    ASSERT_EQ("A", iter.next());
    ASSERT_EQ("few", iter.next());
    ASSERT_EQ("words", iter.next());
    ASSERT_TRUE(iter.is_complete());
}

TEST(StrSplit, split_test_period_delim) {
    Str str = "192.168.0.1";
    Str::Split iter = str.split(".");

    ASSERT_EQ("192", iter.next());
    ASSERT_EQ("168", iter.next());
    ASSERT_EQ("0", iter.next());
    ASSERT_EQ("1", iter.next());
    ASSERT_TRUE(iter.is_complete());
}

TEST(StrSplit, split_with_c_string) {
    const char* stack_str = "I am a stack string, on the stack.";
    Str::Split iter(stack_str);

    ASSERT_EQ("I", iter.next());
    ASSERT_EQ("am", iter.next());
    ASSERT_EQ("a", iter.next());
    ASSERT_EQ("stack", iter.next());
    ASSERT_EQ("string,", iter.next());
    ASSERT_EQ("on", iter.next());
    ASSERT_EQ("the", iter.next());
    ASSERT_EQ("stack.", iter.next());
    ASSERT_TRUE(iter.is_complete());
}

TEST(StrSplit, split_test_string_delim) {
    Str str = "lionAXtigerXYmouseXXYXBBcat";
    Str::Split iter = str.split("ABXY");
    Str tmp;

    tmp = iter.next();
    ASSERT_EQ("lion", tmp);
    ASSERT_EQ(4, tmp.length());
    tmp = iter.next();
    ASSERT_EQ("tiger", tmp);
    ASSERT_EQ(5, tmp.length());
    tmp = iter.next();
    ASSERT_EQ("mouse", tmp);
    ASSERT_EQ(5, tmp.length());
    tmp = iter.next();
    ASSERT_EQ("cat", tmp);
    ASSERT_EQ(3, tmp.length());
}

TEST(StrSplit, split_test_collect) {
    std::vector<std::string> expected;
    expected.push_back("The");
    expected.push_back("quick");
    expected.push_back("brown");
    expected.push_back("fox");
    expected.push_back("jumps");
    expected.push_back("over");
    expected.push_back("the");
    expected.push_back("lazy");
    expected.push_back("dog");
    std::vector<std::string>::iterator itFake;

    Str str = "The quick brown fox jumps over the lazy dog";
    std::vector<std::string> vec2 = str.split().collect<std::vector<std::string> >();
    itFake = expected.begin();
    for (std::vector<std::string>::iterator it = vec2.begin(); it != vec2.end(); it++, itFake++)
        ASSERT_EQ(*it, *itFake);

    std::vector<Str> vec = str.split().collect<std::vector<Str> >();
    itFake = expected.begin();
    for (std::vector<Str>::iterator it = vec.begin(); it != vec.end(); it++, itFake++)
        ASSERT_EQ(*it, *itFake);
}
