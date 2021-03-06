/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   write_and_read.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/20 16:02:15 by rlucas        #+#    #+#                 */
/*   Updated: 2021/05/15 13:52:59 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "gtest/gtest.h"

#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#include "FileDesc.hpp"
#include "Utils.hpp"

TEST(FileDesc_tests, read_and_write_strings) {
    // Writing to a file
    {
        int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        FileDesc file = FileDesc::init(fd);

        file.write("hello there");
    }
    // FileDesc calls close(fd) upon dropping from scope
    // Reading from a file
    {
        int fd = open("output.txt", O_RDONLY, 0644);
        FileDesc file = FileDesc::init(fd);
        std::string str;

        file.read(str, 5);
        EXPECT_EQ(str, "hello");
        EXPECT_EQ(str.size(), 5);
        file.read(str, 10);
        EXPECT_EQ(str, "hello there");
        EXPECT_EQ(str.size(), 11);
    }

    // Remove the temporary file
    EXPECT_EQ(remove("output.txt"), 0);
}

// Test demonstrating that FileDesc uses move semantics for copy
TEST(FileDesc_tests, move_semantics_test) {
    FileDesc fd = FileDesc::init(5);
    FileDesc fd2;

    EXPECT_EQ(fd.raw(), 5);

    fd2 = fd;

    EXPECT_EQ(fd.raw(), 0);
    EXPECT_EQ(fd2.raw(), 5);
}

TEST(FileDesc_tests, read_and_write_buffers) {
    char write_buffer[] = "All your base are belong to us";
    char read_buffer[35];
    Utils::memset(read_buffer, 0, 35);
    {
        int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        FileDesc file = FileDesc::init(fd);

        Utils::RwResult res = file.write(write_buffer, 30);
        EXPECT_TRUE(res.is_ok());
        EXPECT_EQ(res.unwrap(), 30);
    }
    {
        int fd = open("output.txt", O_RDONLY, 0644);
        FileDesc file = FileDesc::init(fd);

        file.read(read_buffer, 30);
        EXPECT_STREQ(read_buffer, "All your base are belong to us");
    }
}

TEST(FileDesc_tests, error_tests) {
    int fd = open("output2.txt", O_WRONLY | O_TRUNC, 0644);
    FileDesc file = FileDesc::init(fd);

    EXPECT_EQ(file.write("hello there"), Utils::RwResult::Err("Bad file descriptor"));
}
