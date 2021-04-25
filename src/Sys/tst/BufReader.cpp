/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   BufReader.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/08 13:43:54 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/21 14:55:55 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../src/BufReader.hpp"
#include "../src/File.hpp"

#include <fcntl.h>

TEST(BufReader, read_until_from_file) {
    {
        File::Result file_res = File::open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

        ASSERT_TRUE(file_res.is_ok());

        File file = file_res.unwrap();
        EXPECT_EQ(file.write("hello there\n, welcome!\n"), Utils::RwResult::Ok(23));
    }
    {
        File::Result file_res = File::open("output.txt", O_RDONLY, 0644);

        ASSERT_TRUE(file_res.is_ok());
        File file = file_res.unwrap();

        BufReader<File> reader = BufReader<File>::init(file);
        std::string line;

        Utils::RwResult res = reader.read_until('\n', line);

        EXPECT_FALSE(res.is_err());
        EXPECT_NE(res.unwrap(), 0);
        EXPECT_EQ(line, "hello there\n");

        line.clear();
        res = reader.read_until('\n', line);

        EXPECT_FALSE(res.is_err());
        EXPECT_NE(res.unwrap(), 0);
        EXPECT_EQ(line, ", welcome!\n");

        line.clear();
        res = reader.read_until('\n', line);

        EXPECT_FALSE(res.is_err());
        EXPECT_EQ(res.unwrap(), 0);
        EXPECT_EQ(line, "");
    }

    EXPECT_EQ(remove("output.txt"), 0);
}

TEST(BufReader, read_line_from_file) {
    {
        File::Result file_res = File::open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

        ASSERT_TRUE(file_res.is_ok());

        File file = file_res.unwrap();
        EXPECT_EQ(file.write("hello there\n, welcome!\n"), Utils::RwResult::Ok(23));
    }
    {
        File::Result file_res = File::open("output.txt", O_RDONLY, 0644);

        ASSERT_TRUE(file_res.is_ok());
        File file = file_res.unwrap();

        BufReader<File> reader = BufReader<File>::init(file);
        std::string line;

        Utils::RwResult res = reader.read_line(line);

        EXPECT_FALSE(res.is_err());
        EXPECT_NE(res.unwrap(), 0);
        EXPECT_EQ(line, "hello there\n");

        line.clear();
        res = reader.read_line(line);

        EXPECT_FALSE(res.is_err());
        EXPECT_NE(res.unwrap(), 0);
        EXPECT_EQ(line, ", welcome!\n");

        line.clear();
        res = reader.read_line(line);

        EXPECT_FALSE(res.is_err());
        EXPECT_EQ(res.unwrap(), 0);
        EXPECT_EQ(line, "");
    }

    EXPECT_EQ(remove("output.txt"), 0);
}
