#include <gtest/gtest.h>

#include "../src/Slice.hpp"

#include <iostream>
#include <vector>

TEST(SliceSplit, split_test_basic) {
    Slice str = "A few words\r\n";
    Slice::Split iter = str.split();

    ASSERT_EQ("A", iter.next());
    ASSERT_EQ("few", iter.next());
    ASSERT_EQ("words", iter.next());
    ASSERT_TRUE(iter.is_complete());
}

TEST(SliceSplit, split_test_period_delim) {
    Slice str = "192.168.0.1";
    Slice::Split iter = str.split(".");

    ASSERT_EQ("192", iter.next());
    ASSERT_EQ("168", iter.next());
    ASSERT_EQ("0", iter.next());
    ASSERT_EQ("1", iter.next());
    ASSERT_TRUE(iter.is_complete());
}

TEST(SliceSplit, count) {
    Slice str = "192.168.0.1";
    Slice::Split iter = str.split(".");

    ASSERT_EQ(iter.count_remaining(), 4);
    ASSERT_EQ("192", iter.next());
    ASSERT_EQ("168", iter.next());
    ASSERT_EQ("0", iter.next());
    ASSERT_EQ("1", iter.next());
    ASSERT_TRUE(iter.is_complete());
}

TEST(SliceSplit, empty_count) {
    Slice str = "";
    Slice::Split iter = str.split();

    ASSERT_EQ(iter.count_remaining(), 0);
}

TEST(SliceSplit, split_with_c_string) {
    const char* stack_str = "I am a stack string, on the stack.";
    Slice::Split iter(stack_str);

    ASSERT_EQ(iter.count_remaining(), 8);
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

TEST(SliceSplit, split_test_string_delim) {
    Slice str = "lionAXtigerXYmouseXXYXBBcat";
    Slice::Split iter = str.split("ABXY");
    Slice tmp;

    ASSERT_EQ(iter.count_remaining(), 4);
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

TEST(SliceSplit, split_test_collect) {
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

    Slice str = "The quick brown fox jumps over the lazy dog";
    std::vector<std::string> vec2 = str.split().collect<std::vector<std::string> >();
    itFake = expected.begin();
    for (std::vector<std::string>::iterator it = vec2.begin(); it != vec2.end(); it++, itFake++)
        ASSERT_EQ(*it, *itFake);

    std::vector<Slice> vec = str.split().collect<std::vector<Slice> >();
    itFake = expected.begin();
    for (std::vector<Slice>::iterator it = vec.begin(); it != vec.end(); it++, itFake++)
        ASSERT_EQ(*it, *itFake);
}

TEST(SliceSplit, split) {
    Slice str("GET / HTTP/1.1\r\n");
    Slice::Split iter = str.split();

    std::vector<Slice> vec = iter.collect<std::vector<Slice> >();

    EXPECT_EQ(vec.size(), 3);
}

TEST(SliceSplit, double_split) {
    Slice str("Host: example.com\r\nAccept: Yes\r\nJunk: mess\r\n");
    Slice::Split iter = str.split("\r\n");

    std::vector<Slice> vec = iter.collect<std::vector<Slice> >();
    ASSERT_EQ(vec.size(), 3);

    Slice middle = vec[1].trim();
    EXPECT_EQ(middle.length(), 11);

    Slice::Split iter2 = middle.split(":");

    ASSERT_EQ(iter2.count_remaining(), 2);
    std::vector<Slice> vec2 = iter2.collect<std::vector<Slice> >();
    ASSERT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0].trim(), "Accept");
    EXPECT_EQ(vec2[1].trim(), "Yes");
}

TEST(SplitN, double_split) {
    Slice str("Host: example.com\r\nAccept: Yes\r\nJunk: mess\r\n");
    Slice::Split iter = str.split("\r\n");

    std::vector<Slice> vec = iter.collect<std::vector<Slice> >();
    ASSERT_EQ(vec.size(), 3);

    Slice middle = vec[1].trim();
    EXPECT_EQ(middle.length(), 11);

    Slice::SplitN iter2 = middle.splitn(1, ":");

    std::vector<Slice> vec2 = iter2.collect<std::vector<Slice> >();
    ASSERT_EQ(vec2.size(), 1);
    EXPECT_EQ(vec2[0], "Accept: Yes");
}

TEST(SplitN, split_once) {
    Slice str("Host: example.com\r\nIpv6 address: 2001:0DB8:AC10:FE01::\r\nJunk: mess\r\n");

    std::vector<Slice> vec = str.split("\r\n").collect<std::vector<Slice> >();
    ASSERT_EQ(vec.size(), 3);

    Slice middle = vec[1].trim();
    std::vector<Slice> vec2 = middle.splitn(2, ":").collect<std::vector<Slice> >();

    ASSERT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0].trim(), "Ipv6 address");
    EXPECT_EQ(vec2[1].trim(), "2001:0DB8:AC10:FE01::");
}

TEST(SplitN, split_header) {
    Slice str("Host: example.com\r\n");
    Slice::SplitN iter = str.splitn(2, ":");
    Slice header_name = iter.next();
    Slice header_value = iter.next();

    ASSERT_TRUE(iter.is_complete());
}
