#include "gtest/gtest.h"
#include "../src/Formula.hpp"

TEST(blatest, test1) {
	EXPECT_EQ(Formula::bla(0), 0);
	EXPECT_EQ(Formula::bla(10), 20);
	EXPECT_EQ(Formula::bla(50), 100);
}
