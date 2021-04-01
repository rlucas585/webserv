/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   optional.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/01 17:16:24 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/01 20:46:51 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/optional.hpp"
#include <gtest/gtest.h>

#include <iostream>
#include <vector>

// Struct that enforces usage of the heap, to test Utils::optional manages
// memory correctly
struct HeapMemData {
    HeapMemData(void) : data(new char[100]) {}
    ~HeapMemData(void) {
        if (data) {
            delete[] data;
        }
    }
    HeapMemData(HeapMemData const& other) : data(0) {
        (void)other;
        if (data)
            delete[] data;
        data = new char[100];
    }
    HeapMemData& operator=(HeapMemData const& rhs) {
        if (this == &rhs) {
            return *this;
        }
        if (data)
            delete[] data;
        data = new char[100];
        return *this;
    }

    char* data;
};

TEST(optional, basic_test) {
    Utils::optional<int> opt;

    EXPECT_FALSE(opt.has_value());
}

TEST(optional, nullopt_t) {
    Utils::optional<int> opt1 = Utils::nullopt;
    Utils::optional<int> opt2(Utils::nullopt);

    EXPECT_FALSE(opt1.has_value());
    EXPECT_FALSE(opt2.has_value());
}

TEST(optional, nullopt_as_unset) {
    Utils::optional<int> opt1(20);

    EXPECT_TRUE(opt1.has_value());
    EXPECT_EQ(opt1.value(), 20);
    opt1 = Utils::nullopt;

    EXPECT_FALSE(opt1.has_value());
}

TEST(optional, value_throw) {
    Utils::optional<int> opt(Utils::nullopt);
    EXPECT_THROW(
        {
            try {
                EXPECT_EQ(opt.value(), 10);
            } catch (Utils::runtime_error const& err) {
                EXPECT_STREQ("Bad optional access", err.what());
                throw;
            }
        },
        Utils::runtime_error);
}

TEST(optional, unwrap_expect) {
    Utils::optional<int> opt1(5);
    Utils::optional<int> opt2(25);
    int x = opt1.unwrap();
    int y = opt2.expect("No value in opt2");

    EXPECT_EQ(x, 5);
    EXPECT_EQ(y, 25);
    EXPECT_FALSE(opt1.has_value());
    EXPECT_FALSE(opt2.has_value());
    EXPECT_THROW(
        {
            try {
                EXPECT_EQ(opt1.value(), 10);
            } catch (Utils::runtime_error const& err) {
                EXPECT_STREQ("Bad optional access", err.what());
                throw;
            }
        },
        Utils::runtime_error);
}

TEST(optional, unwrap_expect_throw) {
    Utils::optional<int> opt(Utils::nullopt);
    EXPECT_THROW(
        {
            try {
                EXPECT_EQ(opt.unwrap(), 10);
            } catch (Utils::runtime_error const& err) {
                EXPECT_STREQ("Bad optional access", err.what());
                throw;
            }
        },
        Utils::runtime_error);
    EXPECT_THROW(
        {
            try {
                EXPECT_EQ(opt.expect("No value in opt"), 10);
            } catch (Utils::runtime_error const& err) {
                EXPECT_STREQ("No value in opt", err.what());
                throw;
            }
        },
        Utils::runtime_error);
}

TEST(optional, assign_value) {
    Utils::optional<int> opt(5);

    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(*opt, 5);
    EXPECT_EQ(opt.value(), 5);
}

// Demonstrate that heap memory is handled properly
TEST(optional, heap_value) {
    Utils::optional<HeapMemData> opt;
    HeapMemData data;

    EXPECT_FALSE(opt.has_value());

    opt.set(data);
    EXPECT_TRUE(opt.has_value());
}

TEST(optional, make_optional) {
    std::string str("This is a string inside of an option");
    Utils::optional<std::string> opt = Utils::make_optional(str);

    EXPECT_EQ(*opt, str);
    EXPECT_EQ(opt->size(), str.size());
}

TEST(optional, value_or) {
    std::string str("Vim > Visual Studio Code");
    Utils::optional<std::string> opt;

    EXPECT_EQ(opt.value_or("Wish I could use move semantics"), "Wish I could use move semantics");
    opt = Utils::make_optional(str);
    EXPECT_EQ(opt.value_or("Wish I could use move semantics"), "Vim > Visual Studio Code");
}

TEST(optional, int_value_or) {
    Utils::optional<int> opt;

    EXPECT_EQ(opt.value_or(1), 1);
    opt = Utils::make_optional(5);
    EXPECT_EQ(opt.value_or(1), 5);
}

TEST(optional, swap_test1) {
    Utils::optional<int> opt1(20);
    Utils::optional<int> opt2;

    EXPECT_TRUE(opt1.has_value());
    EXPECT_FALSE(opt2.has_value());
    EXPECT_EQ(opt1.value(), 20);

    opt1.swap(opt2);

    EXPECT_FALSE(opt1.has_value());
    EXPECT_TRUE(opt2.has_value());
    EXPECT_EQ(opt2.value(), 20);
}

TEST(optional, swap_test2) {
    Utils::optional<std::string> opt1(std::string("Somewhat long string that should heap allocated"));
    Utils::optional<std::string> opt2;

    EXPECT_TRUE(opt1.has_value());
    EXPECT_FALSE(opt2.has_value());
    EXPECT_EQ(opt1.value(), "Somewhat long string that should heap allocated");

    opt1.swap(opt2);

    EXPECT_FALSE(opt1.has_value());
    EXPECT_TRUE(opt2.has_value());
    EXPECT_EQ(*opt2, "Somewhat long string that should heap allocated");
}

TEST(optional, reset) {
    Utils::optional<int> opt1(20);

    EXPECT_TRUE(opt1.has_value());
    opt1.reset();
    EXPECT_FALSE(opt1.has_value());
}

template <typename T> static Utils::optional<T> get_val_at_position(std::vector<T> const& vec, size_t index) {
    if (index >= vec.size())
        return Utils::optional<int>();
    return Utils::make_optional(vec[index]);
}

TEST(optional, function_return) {
    std::vector<int> vec = {1, 3, 5, 7, 9, 11};
    Utils::optional<int> opt1 = get_val_at_position(vec, 3);
    Utils::optional<int> opt2 = get_val_at_position(vec, 7);

    EXPECT_TRUE(opt1.has_value());
    EXPECT_FALSE(opt2.has_value());
    EXPECT_EQ(opt1.value(), 7);
}
