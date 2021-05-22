#include <gtest/gtest.h>

#include "Slice.hpp"
#include "Utils.hpp"

#include <iostream>
#include <sstream>

// clang-format off
#define TEST_THROW(FUNC, ERRMSG) \
    EXPECT_THROW( \
        { \
            try { \
                FUNC \
            } catch (Utils::runtime_error const& err) { \
                EXPECT_STREQ(ERRMSG, err.what()); \
                throw; \
            } \
        }, \
        Utils::runtime_error);
// clang-format on

TEST(SliceCreation, default_construction) {
    Slice str;

    ASSERT_FALSE(str.isInitialized());
    TEST_THROW(str.length();, "Operation called using Uninitialized Slice");
}

TEST(SliceCreation, construction_of_empty_str) {
    Slice str("");

    ASSERT_TRUE(str.isInitialized());
    ASSERT_EQ(str.length(), 0);
}

TEST(SliceCreation, construction_from_char_pointer) {
    Slice str("hello there");
    Slice str2 = "hello there";

    ASSERT_TRUE(str.isInitialized());
    ASSERT_EQ(str.length(), 11);
    ASSERT_TRUE(str2.isInitialized());
    ASSERT_EQ(str2.length(), 11);
}

TEST(SliceCreation, construction_from_string) {
    std::string hi("hello there");
    Slice str(hi);
    Slice str2 = hi;

    ASSERT_TRUE(str.isInitialized());
    ASSERT_EQ(str.length(), 11);
    ASSERT_TRUE(str2.isInitialized());
    ASSERT_EQ(str2.length(), 11);
}

TEST(SliceCreation, output_test) {
    std::stringstream stream1;
    std::stringstream stream2;
    std::stringstream stream3;
    Slice str("hello there");
    Slice str2;
    Slice str3("");

    stream1 << str;
    ASSERT_EQ(stream1.str(), "hello there");
    stream2 << str2;
    ASSERT_EQ(stream2.str(), "null");
    stream3 << str3;
    ASSERT_EQ(stream3.str(), "");
}

TEST(SliceCreation, construction_of_partial_slice) {
    Slice str = Slice::newSliceWithLength("hello there", 5);
    Slice str2 = Slice::newSliceWithLengthAndOffset("hello there", 5, 6);
    std::stringstream stream1;
    std::stringstream stream2;

    ASSERT_EQ(str.length(), 5);
    stream1 << str;
    ASSERT_EQ(stream1.str(), "hello");
    ASSERT_EQ(str2.length(), 5);
    stream2 << str2;
    ASSERT_EQ(stream2.str(), "there");
}

TEST(SliceCreation, construction_of_slices_from_others_with_offset) {
    Slice str("I am a string slice");
    Slice str2 = Slice::newSliceWithOffset(str, 7);

    ASSERT_EQ(str2, "string slice");
}

TEST(Slice_tests, count_test) {
    Slice str = "count the number of e's in this Slice please";

    EXPECT_EQ(str.count('e'), 6);
    EXPECT_EQ(str.count('u'), 2);
}

TEST(Slice_tests, find_test) {
    Slice haystack = "This string has a lot of needle's to find";

    Utils::optional<Slice> search = haystack.find("string");

    ASSERT_TRUE(search.has_value());
    EXPECT_EQ(search.unwrap(), "string has a lot of needle's to find");

    search = haystack.find("of need");

    ASSERT_TRUE(search.has_value());
    EXPECT_EQ(search.unwrap(), "of needle's to find");

    search = haystack.find("spiderman");

    ASSERT_FALSE(search.has_value());
}

TEST(Slice_tests, front_test) {
    Slice str;
    Slice str2 = "actual slice";

    EXPECT_FALSE(str.front().has_value());
    ASSERT_TRUE(str2.front().has_value());
    EXPECT_EQ(str2.front().unwrap(), 'a');
}
