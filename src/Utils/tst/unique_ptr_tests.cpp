/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unique_ptr_tests.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/23 21:26:07 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/23 22:47:26 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/Utils.hpp"
#include <gtest/gtest.h>

#include <iostream>
#include <string>

// Run with valgrind to confirm unique_ptr capabilities

TEST(Utils_tests, unique_ptr_test) {
    {
        Utils::unique_ptr<std::string> ptr(new std::string());

        // Raw pointers cause leaks, unique_ptr does not
        // std::string                         *i_cause_leaks = new std::string("hello there");
        // EXPECT_EQ(*i_cause_leaks, "hello there");

        ptr->append("hello there");
        EXPECT_EQ(*ptr, "hello there");

        ptr->append("make the string longer than 24 bytes so that the std::"
                    "string has to allocate heap memory");
    }
}

TEST(Utils_tests, unique_ptr_swap) {
    Utils::unique_ptr<std::string> ptr(new std::string("hello there"));
    Utils::unique_ptr<std::string> ptr2;

    ptr2.swap(ptr);
    EXPECT_EQ(*ptr2, "hello there");
}

TEST(Utils_tests, unique_ptr_get) {
    Utils::unique_ptr<std::string> ptr(new std::string("hello there"));
    std::string* rawptr;

    rawptr = ptr.get();
    EXPECT_EQ(*rawptr, "hello there");
}

TEST(Utils_tests, unique_ptr_reset) {
    Utils::unique_ptr<std::string> ptr(new std::string("hello there"));
    std::string* rawptr = new std::string("goodbye");

    EXPECT_EQ(*ptr, "hello there");
    ptr.reset(rawptr);
    EXPECT_EQ(*ptr, "goodbye");
}
