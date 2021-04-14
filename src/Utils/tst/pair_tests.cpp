/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pair_tests.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 16:32:06 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/27 16:34:59 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/pair.hpp"
#include <gtest/gtest.h>

TEST(Utils_tests, pair_comparison) {
    Utils::pair<int, int> p1 = Utils::make_pair(5, 10);
    Utils::pair<int, int> p2 = Utils::make_pair(5, 10);

    EXPECT_EQ(p1, p2);
}

TEST(Utils_tests, pair_copy) {
    Utils::pair<int, int> p1 = Utils::make_pair(5, 10);
    Utils::pair<int, int> p2(p1);
    Utils::pair<int, int> p3 = p2;

    EXPECT_EQ(p1, p2);
    EXPECT_EQ(p1, p3);
    EXPECT_EQ(p2, p3);
}
