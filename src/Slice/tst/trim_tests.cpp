/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   trim_tests.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 11:58:37 by rlucas        #+#    #+#                 */
/*   Updated: 2021/05/15 13:51:12 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "Slice.hpp"
#include "Utils.hpp"

#include <iostream>

TEST(Slice, trim) {
    Slice str = "     Vim      ";

    EXPECT_EQ(str.length(), 14);
    EXPECT_EQ(str, "     Vim      ");

    str.trim();

    EXPECT_EQ(str.length(), 3);
    EXPECT_EQ(str, "Vim");
}

TEST(Slice, trim_all_whitespace) {
    Slice str = " \t\t\r    Vim    \v\v\n\n  ";

    EXPECT_EQ(str.length(), 21);
    EXPECT_EQ(str, " \t\t\r    Vim    \v\v\n\n  ");

    str.trim();

    EXPECT_EQ(str.length(), 3);
    EXPECT_EQ(str, "Vim");
}

TEST(Slice, trim_with_parameter) {
    Slice str = "XXXXXVimXXXXXX";

    EXPECT_EQ(str.length(), 14);
    EXPECT_EQ(str, "XXXXXVimXXXXXX");

    str.trim("X");

    EXPECT_EQ(str.length(), 3);
    EXPECT_EQ(str, "Vim");
}

TEST(Slice, trim_string) {
    Slice str = "XxXxXxWoxrdsXxXxXx";

    EXPECT_EQ(str.length(), 18);
    EXPECT_EQ(str, "XxXxXxWoxrdsXxXxXx");

    str.trim("Xx");

    EXPECT_EQ(str.length(), 6);
    EXPECT_EQ(str, "Woxrds");
}
