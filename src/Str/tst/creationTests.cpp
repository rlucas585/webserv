#include <gtest/gtest.h>

#include "Str.hpp"

#include <iostream>
#include <sstream>

TEST(StrCreation, default_construction) {
	Str			str;

	ASSERT_FALSE(str.isInitialized());
	ASSERT_THROW(str.length(), std::exception);
}

TEST(StrCreation, construction_from_null) {
	Str			str = NULL;
	Str			str2(NULL);

	ASSERT_FALSE(str.isInitialized());
	ASSERT_THROW(str.length(), std::exception);
	ASSERT_FALSE(str2.isInitialized());
	ASSERT_THROW(str2.length(), std::exception);
}

TEST(StrCreation, construction_of_empty_str) {
	Str			str("");

	ASSERT_TRUE(str.isInitialized());
	ASSERT_EQ(str.length(), 0);
}

TEST(StrCreation, construction_from_char_pointer) {
	Str			str("hello there");
	Str			str2 = "hello there";

	ASSERT_TRUE(str.isInitialized());
	ASSERT_EQ(str.length(), 11);
	ASSERT_TRUE(str2.isInitialized());
	ASSERT_EQ(str2.length(), 11);
}

TEST(StrCreation, construction_from_string) {
	std::string		hi("hello there");
	Str				str(hi);
	Str				str2 = hi;

	ASSERT_TRUE(str.isInitialized());
	ASSERT_EQ(str.length(), 11);
	ASSERT_TRUE(str2.isInitialized());
	ASSERT_EQ(str2.length(), 11);
}

TEST(StrCreation, output_test) {
	std::stringstream		stream1;
	std::stringstream		stream2;
	std::stringstream		stream3;
	Str			str("hello there");
	Str			str2;
	Str			str3("");

	stream1 << str;
	ASSERT_EQ(stream1.str(), "hello there");
	stream2 << str2;
	ASSERT_EQ(stream2.str(), "null");
	stream3 << str3;
	ASSERT_EQ(stream3.str(), "");
}

TEST(StrCreation, construction_of_partial_slice) {
	Str			str = Str::newSliceWithLength("hello there", 5);
	Str			str2 = Str::newSliceWithLengthAndOffset("hello there", 5, 6);
	std::stringstream		stream1;
	std::stringstream		stream2;

	ASSERT_EQ(str.length(), 5);
	stream1 << str;
	ASSERT_EQ(stream1.str(), "hello");
	ASSERT_EQ(str2.length(), 5);
	stream2 << str2;
	ASSERT_EQ(stream2.str(), "there");
}

TEST(StrCreation, construction_of_slices_from_others_with_offset) {
	Str			str("I am a string slice");
	Str			str2 = Str::newSliceWithOffset(str, 7);

	ASSERT_EQ(str2, "string slice");
}
