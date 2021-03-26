/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   basic_tests.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/17 19:08:10 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/26 21:14:49 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/Utils.hpp"
#include <cstring>
#include <gtest/gtest.h>

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

TEST(Utils_tests, memset) {
    char buf[10];
    char buf2[10];

    Utils::memset(buf, 15, 10);
    memset(buf2, 15, 10);
    for (size_t i = 0; i < 10; i++) {
        EXPECT_EQ(buf[i], buf2[i]);
    }
}

TEST(Utils_tests, atoi) {
    int num = Utils::atoi("15");

    ASSERT_EQ(num, 15);
}

TEST(Utils_tests, min) {
    char str[] = "Test";
    char str2[] = "Rest";
    int x = 15;
    int y = 20;
    std::string s1("Hello");
    std::string s2("Henlo");

    EXPECT_EQ(Utils::min(str, str2), std::min(str, str2));
    EXPECT_EQ(Utils::min(x, y), std::min(x, y));
    EXPECT_EQ(Utils::min(s1, s2), std::min(s1, s2));
}

TEST(Utils_tests, max) {
    char str[] = "Test";
    char str2[] = "Rest";
    int x = 15;
    int y = 20;
    std::string s1("Hello");
    std::string s2("Henlo");

    EXPECT_EQ(Utils::max(str, str2), std::max(str, str2));
    EXPECT_EQ(Utils::max(x, y), std::max(x, y));
    EXPECT_EQ(Utils::max(s1, s2), std::max(s1, s2));
}
