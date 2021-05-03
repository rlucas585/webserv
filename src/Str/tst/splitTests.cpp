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

TEST(StrSplit, split) {
    Str str("GET / HTTP/1.1\r\n");
    Str::Split iter = str.split();

    std::vector<Str> vec = iter.collect<std::vector<Str> >();

    EXPECT_EQ(vec.size(), 3);
}

TEST(StrSplit, double_split) {
    Str str("Host: example.com\r\nAccept: Yes\r\nJunk: mess\r\n");
    Str::Split iter = str.split("\r\n");

    std::vector<Str> vec = iter.collect<std::vector<Str> >();
    ASSERT_EQ(vec.size(), 3);

    Str middle = vec[1].trim();
    EXPECT_EQ(middle.length(), 11);

    Str::Split iter2 = middle.split(":");

    std::vector<Str> vec2 = iter2.collect<std::vector<Str> >();
    ASSERT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0].trim(), "Accept");
    EXPECT_EQ(vec2[1].trim(), "Yes");
}

TEST(SplitN, double_split) {
    Str str("Host: example.com\r\nAccept: Yes\r\nJunk: mess\r\n");
    Str::Split iter = str.split("\r\n");

    std::vector<Str> vec = iter.collect<std::vector<Str> >();
    ASSERT_EQ(vec.size(), 3);

    Str middle = vec[1].trim();
    EXPECT_EQ(middle.length(), 11);

    Str::SplitN iter2 = middle.splitn(1, ":");

    std::vector<Str> vec2 = iter2.collect<std::vector<Str> >();
    ASSERT_EQ(vec2.size(), 1);
    EXPECT_EQ(vec2[0], "Accept: Yes");
}

TEST(SplitN, split_once) {
  Str str("Host: example.com\r\nIpv6 address: 2001:0DB8:AC10:FE01::\r\nJunk: mess\r\n");

  std::vector<Str> vec = str.split("\r\n").collect<std::vector<Str> >();
  ASSERT_EQ(vec.size(), 3);

  Str middle = vec[1].trim();
  std::vector<Str> vec2 = middle.splitn(2, ":").collect<std::vector<Str> >();

  ASSERT_EQ(vec2.size(), 2);
  EXPECT_EQ(vec2[0].trim(), "Ipv6 address");
  EXPECT_EQ(vec2[1].trim(), "2001:0DB8:AC10:FE01::");
}
