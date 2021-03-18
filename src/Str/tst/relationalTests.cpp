#include <gtest/gtest.h>

#include "Str.hpp"

#include <iostream>

TEST(StrComparisons, equality_with_Str) {
	Str			str("hello there");
	Str			str2("hello there");

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

TEST(StrComparisons, equality_with_std_string) {
	Str			str("hello there");
	Str			str2("hello shere");

	std::string	realStr("hello there");
	ASSERT_TRUE(str == realStr);
	ASSERT_TRUE(realStr == str);
	ASSERT_TRUE(str2 <= realStr);
	ASSERT_TRUE(str2 < realStr);
	ASSERT_TRUE(realStr >= str2);
	ASSERT_TRUE(realStr > str2);

}

TEST(StrComparisons, equality_with_c_strings) {
	Str			str("hello there");
	Str			str2("hello shere");

	const char	*staticStr = "hello there";
	ASSERT_TRUE(str == staticStr);
	ASSERT_TRUE(staticStr == str);
	ASSERT_TRUE(str2 <= staticStr);
	ASSERT_TRUE(str2 < staticStr);
	ASSERT_TRUE(staticStr >= str2);
	ASSERT_TRUE(staticStr > str2);
}

TEST(StrComparisons, equality_of_empty_strings) {
	const char *	staticStr = "";
	Str				str("");
	Str				str2("h");

	ASSERT_EQ(str, staticStr);
	ASSERT_TRUE(str2 > str);
}

TEST(StrComparisons, equality_of_partial_slices) {
	const char *	staticStr = "hello goodbye hello";
	Str				str = Str::newSliceWithLength(staticStr, 5);
	Str				str2 = Str::newSliceWithLengthAndOffset(staticStr, 7, 6);
	Str				str3 = Str::newSliceWithLengthAndOffset(staticStr, 5, 14);

	ASSERT_EQ(str, str3);
}
