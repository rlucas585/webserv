/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   iterationTests.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/26 21:42:39 by rlucas        #+#    #+#                 */
/*   Updated: 2021/05/15 13:50:50 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "Slice.hpp"
#include "Utils.hpp"

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

TEST(SliceIteration, basic_iteration) {
    const char* staticSlice = "hello there";
    Slice str(staticSlice);

    size_t i = 0;
    for (Slice::iterator it = str.begin(); it != str.end(); it++, i++) {
        ASSERT_EQ(staticSlice[i], *it);
    }
}

TEST(SliceIteration, iteration_throw) {
    const char* staticSlice = "hello there";
    Slice str(staticSlice);

    Slice::iterator it = str.end();
    TEST_THROW(*it;, "Dereferencing iterator past end of Slice");
}
