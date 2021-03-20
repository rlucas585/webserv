/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   write_and_read.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/20 16:02:15 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/20 17:12:14 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "gtest/gtest.h"

#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include "../src/FileDesc.hpp"
#include "../../Utils/src/Utils.hpp"

TEST(FileDesc_tests, read_and_write) {
    // Writing to a file
    {
        int         fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        FileDesc    file = FileDesc::init(fd);

        file.writeToFile("hello there");
    }
    // FileDesc calls close(fd) upon dropping from scope
    // Reading from a file
    {
        int         fd = open("output.txt", O_RDONLY, 0644);
        FileDesc    file = FileDesc::init(fd);
        std::string str;

        file.readFromFile(str, 5);
        EXPECT_EQ(str, "hello");
        EXPECT_EQ(str.size(), 5);
        file.readFromFile(str, 10);
        EXPECT_EQ(str, "hello there");
        EXPECT_EQ(str.size(), 11);
    }

    // Remove the temporary file
    EXPECT_EQ(remove("output.txt"), 0);
}

TEST(FileDesc_tests, error_tests) {
    int         fd = open("output2.txt", O_WRONLY | O_TRUNC, 0644);
    FileDesc    file = FileDesc::init(fd);

    EXPECT_THROW({
            try {
            file.writeToFile("hello there");
            } catch (Utils::runtime_error const& err) {
            EXPECT_STREQ("FileDesc::writeToFile Error: Bad file descriptor", err.what());
            throw ;
            }
            }, Utils::runtime_error);
}
