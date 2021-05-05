#include "../src/Utils.hpp"
#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include <algorithm>
#include <type_traits>
#include <vector>

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

TEST(Utils_tests, unique_ptr_array) { Utils::array_unique_ptr<char> ptr(new char[1000]); }

TEST(Utils_tests, unique_ptr_in_vector) {
    std::vector<Utils::unique_ptr<int> > vec;

    static_assert(std::is_copy_assignable<Utils::unique_ptr<int> >(),
                  "Utils::unique_ptr is copy-assignable due to no move semantics in C++98");
    vec.push_back(Utils::unique_ptr<int>(new int(1)));
    vec.push_back(Utils::unique_ptr<int>(new int(2)));
    vec.push_back(Utils::unique_ptr<int>(new int(3)));

    // Utils::unique_ptr compares value of another Utils::unique_ptr with ==
    std::vector<Utils::unique_ptr<int> >::iterator iter =
        std::find(vec.begin(), vec.end(), Utils::unique_ptr<int>(new int(2)));
    EXPECT_NE(iter, vec.end());
    EXPECT_EQ(**iter, 2);
}

TEST(reference_test, unique_ptr_in_vector) {
    std::vector<std::unique_ptr<int> > vec;

    static_assert(
        !std::is_copy_assignable<std::unique_ptr<int> >(),
        "std::unique_ptr is correctly not copy-assignable as move semantics exist in C++11");
    vec.push_back(std::unique_ptr<int>(new int(1)));
    vec.push_back(std::unique_ptr<int>(new int(2)));
    vec.push_back(std::unique_ptr<int>(new int(3)));

    // std::unique_ptr compares address of pointer with ==
    std::vector<std::unique_ptr<int> >::iterator iter =
        std::find(vec.begin(), vec.end(), std::unique_ptr<int>(new int(2)));
    EXPECT_EQ(iter, vec.end());
}
