/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   basic_tests.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/17 19:08:10 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/19 11:36:03 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/Utils.hpp"
#include <cstring>
#include <gtest/gtest.h>
#include <iostream>

TEST(Utils_tests, strlen) {
    EXPECT_EQ(Utils::strlen(""), strlen(""));
    EXPECT_EQ(Utils::strlen("hello"), strlen("hello"));
    EXPECT_EQ(Utils::strlen("goodbye"), strlen("goodbye"));
    EXPECT_EQ(Utils::strlen("111112222233333444445555566666777778888899999"),
              strlen("111112222233333444445555566666777778888899999"));
}

TEST(Utils_tests, strchr) {
    const char str[] = "This is a sample string";
    const char str2[] = "Vim Good ; Visual Studio Code Bad";

    EXPECT_EQ(Utils::strchr(str, 's'), strchr(str, 's'));
    EXPECT_EQ(Utils::strchr(str2, 'C'), strchr(str2, 'C'));

    char volstr[] = "A volatile string";
    char volstr2[] = "A volatile string";

    *(Utils::strchr(volstr, 't')) = 'X';
    *(strchr(volstr2, 't')) = 'X';
    EXPECT_TRUE(strcmp(volstr, volstr2) == 0);
}

TEST(Utils_tests, strcmp) {
    const char str[] = "This is a sample string";
    const char str2[] = "Vim Good ; Visual Studio Code Bad";
    const char str3[] = "not that long";
    const char str4[] = "not that longer";

    EXPECT_EQ(Utils::strcmp(str, str2) > 0, strcmp(str, str2) > 0);
    EXPECT_EQ(Utils::strcmp(str, str) > 0, strcmp(str, str) > 0);
    EXPECT_EQ(Utils::strcmp(str, str4) > 0, strcmp(str, str4) > 0);
    EXPECT_EQ(Utils::strcmp(str4, str) > 0, strcmp(str4, str) > 0);
    EXPECT_EQ(Utils::strcmp(str4, str3) > 0, strcmp(str4, str3) > 0);
    EXPECT_EQ(Utils::strcmp(str3, str4) > 0, strcmp(str3, str4) > 0);
}

TEST(Utils_tests, strncmp) {
    const char str[] = "This is a sample string";
    const char str2[] = "Vim Good ; Visual Studio Code Bad";
    const char str3[] = "not that long";
    const char str4[] = "not that longer";
    const char str5[] = "hello goodbye hello";
    const char str6[] = "helloa goodbye hello";

    EXPECT_EQ(Utils::strncmp(str, str2, 5) == 0, strncmp(str, str2, 5) == 0);
    EXPECT_EQ(Utils::strncmp(str, str, 14) == 0, strncmp(str, str, 14) == 0);
    EXPECT_EQ(Utils::strncmp(str, str4, 7) == 0, strncmp(str, str4, 7) == 0);
    EXPECT_EQ(Utils::strncmp(str4, str, 0) == 0, strncmp(str4, str, 0) == 0);
    EXPECT_EQ(Utils::strncmp(str4, str3, 3) == 0, strncmp(str4, str3, 3) == 0);
    EXPECT_EQ(Utils::strncmp(str3, str4, 9) == 0, strncmp(str3, str4, 9) == 0);

    EXPECT_EQ(Utils::strncmp(str5, str6, 5) == 0, strncmp(str5, str6, 5) == 0);
}

TEST(Utils_tests, strpbrk) {
    char str[] = "This is a sample string";
    char key[] = "aeiou";
    char* test;
    char* real;
    test = Utils::strpbrk(str, key);
    real = strpbrk(str, key);
    while (test != 0) {
        EXPECT_TRUE(strcmp(test, real) == 0);
        test = Utils::strpbrk(test + 1, key);
        real = strpbrk(real + 1, key);
    }
}
