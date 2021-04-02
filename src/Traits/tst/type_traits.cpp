/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   type_traits.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 12:12:53 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/02 17:37:08 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include <iostream>
#include <type_traits>

#include "../src/type_traits.hpp"

TEST(Traits, integral_constant_int) {
    size_t actual = meta::integral_constant<int, 2>::value;
    size_t expected = std::integral_constant<int, 2>::value;
    size_t actual2 = meta::integral_constant<int, 4>::value;
    size_t expected2 = std::integral_constant<int, 4>::value;

    EXPECT_EQ(actual, expected);
    EXPECT_EQ(actual2, expected2);
}

TEST(Traits, integral_constant_size_t) {
    size_t actual = meta::integral_constant<size_t, -1>::value;
    size_t expected = std::integral_constant<size_t, -1>::value;
    size_t actual2 = meta::integral_constant<size_t, 50>::value;
    size_t expected2 = std::integral_constant<size_t, 50>::value;

    EXPECT_EQ(actual, expected);
    EXPECT_EQ(actual2, expected2);
}

template <typename T> class Traits : public ::testing::Test {};

typedef testing::Types<int, char, std::string, double> Implementations;

TYPED_TEST_CASE(Traits, Implementations);

TYPED_TEST(Traits, alignment_of) {
    size_t actual = meta::alignment_of<TypeParam>::value;
    size_t expected = std::alignment_of<TypeParam>::value;

    EXPECT_EQ(actual, expected);
}

TEST(Traits, if_c) {
    typedef meta::if_c<true, char, int>::type t1;
    typedef meta::if_c<false, char, int>::type t2;

    EXPECT_EQ(sizeof(t1), sizeof(char));
    EXPECT_EQ(sizeof(t2), sizeof(int));
}

struct Data {
    char data[100];
};

TEST(Traits, type_with_alignment_int) {
    typedef typename meta::type_with_alignment<meta::alignment_of<int>::value>::type aligner;

    EXPECT_EQ(sizeof(aligner), 4);
}

TEST(Traits, type_with_alignment_string) {
    typedef typename meta::type_with_alignment<meta::alignment_of<std::string>::value>::type aligner;

    EXPECT_EQ(sizeof(aligner), 8);
}

TEST(Traits, type_with_alignment_data) {
    typedef typename meta::type_with_alignment<meta::alignment_of<Data>::value>::type aligner;

    EXPECT_EQ(sizeof(aligner), 1);
}

TEST(Traits, type_with_alignment_data_custom) {
    typedef typename meta::type_with_alignment<8>::type aligner;

    EXPECT_EQ(sizeof(aligner), 8);
}
