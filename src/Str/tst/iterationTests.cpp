#include <gtest/gtest.h>

#include "../src/Str.hpp"
#include "../../Utils/src/Utils.hpp"

#define TEST_THROW(FUNC,ERRMSG) EXPECT_THROW({ \
            try { \
            FUNC \
            } catch (Utils::runtime_error const& err) { \
            EXPECT_STREQ(ERRMSG, err.what()); \
            throw ; \
            } \
            }, Utils::runtime_error);

TEST(StrIteration, basic_iteration) {
    const char* staticStr = "hello there";
    Str str(staticStr);

    size_t i = 0;
    for (Str::iterator it = str.begin(); it != str.end(); it++, i++) {
        ASSERT_EQ(staticStr[i], *it);
    }
}

TEST(StrIteration, iteration_throw) {
    const char* staticStr = "hello there";
    Str str(staticStr);

    Str::iterator it = str.end();
    TEST_THROW(*it;, "Dereferencing iterator past end of Str");
}
