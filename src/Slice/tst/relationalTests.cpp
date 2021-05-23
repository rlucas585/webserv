#include <gtest/gtest.h>

#include "Slice.hpp"
#include "Utils.hpp"

#include <cstring>
#include <iostream>

TEST(SliceComparisons, equality_with_Slice) {
    Slice str("hello there");
    Slice str2("hello there");

    ASSERT_TRUE(str == str2);
    ASSERT_FALSE(str != str2);

    str2 = ("hello shere");
    ASSERT_FALSE(str == str2);
    ASSERT_TRUE(str != str2);
    ASSERT_TRUE(str >= str2);
    ASSERT_TRUE(str > str2);
    ASSERT_FALSE(str < str2);
    ASSERT_FALSE(str <= str2);
}

TEST(SliceComparisons, equality_with_std_string) {
    Slice str("hello there");
    Slice str2("hello shere");

    std::string realSlice("hello there");
    ASSERT_TRUE(str == realSlice);
    ASSERT_TRUE(realSlice == str);
    ASSERT_TRUE(str2 <= realSlice);
    ASSERT_TRUE(str2 < realSlice);
    ASSERT_TRUE(realSlice >= str2);
    ASSERT_TRUE(realSlice > str2);
}

TEST(SliceComparisons, equality_with_c_strings) {
    Slice str("hello there");
    Slice str2("hello shere");

    const char* staticSlice = "hello there";
    ASSERT_TRUE(str == staticSlice);
    ASSERT_TRUE(staticSlice == str);
    ASSERT_TRUE(str2 <= staticSlice);
    ASSERT_TRUE(str2 < staticSlice);
    ASSERT_TRUE(staticSlice >= str2);
    ASSERT_TRUE(staticSlice > str2);
}

TEST(SliceComparisons, equality_of_empty_strings) {
    const char* staticSlice = "";
    Slice str("");
    Slice str2("h");

    ASSERT_EQ(str, staticSlice);
    ASSERT_TRUE(str2 > str);
}

TEST(SliceComparisons, equality_of_partial_slices) {
    const char* staticSlice = "hello goodbye hello";
    Slice str = Slice::newSliceWithLength(staticSlice, 5);
    Slice str2 = Slice::newSliceWithLengthAndOffset(staticSlice, 7, 6);
    Slice str3 = Slice::newSliceWithLengthAndOffset(staticSlice, 5, 14);

    ASSERT_EQ(str, str3);
}
