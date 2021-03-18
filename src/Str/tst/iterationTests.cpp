#include <gtest/gtest.h>

#include "Str.hpp"

TEST(StrIteration, basic_iteration) {
	const char *	staticStr = "hello there";
	Str		str(staticStr);

	size_t	i = 0;
	for (Str::iterator it = str.begin(); it != str.end(); it++, i++) {
		ASSERT_EQ(staticStr[i], *it);
	}
}
