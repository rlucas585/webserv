/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   result.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/01 21:21:50 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/01 23:10:18 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/result.hpp"
#include <gtest/gtest.h>

// This file could potentially have more testing, but while implementation
// of result was non-trivial, it doesnt' have many member functions other than
// is_ok, is_err, unwrap and expect.

// class to test correct memory management of result with valgrind
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

Utils::result<int, std::string> add_to_limit_of_ten(int x, int y) {
    typedef Utils::result<int, std::string> ResultType;
    int ret = x + y;

    if (ret > 10)
        return ResultType::Err("Over limit of 10");
    else
        return ResultType::Ok(ret);
}

TEST(Result, unwrapping) {
    Utils::result<int, std::string> res1 = add_to_limit_of_ten(5, 3);
    Utils::result<int, std::string> res2 = add_to_limit_of_ten(5, 6);

    EXPECT_TRUE(res1.is_ok());
    EXPECT_EQ(res1.unwrap(), 8);

    EXPECT_TRUE(res2.is_err());
    EXPECT_EQ(res2.unwrap_err(), "Over limit of 10");
}

TEST(Result, memory_management) {
    HeapMemData data;
    Utils::result<HeapMemData, std::string> res(data);

    EXPECT_TRUE(res.is_ok());
}
