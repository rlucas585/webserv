/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   runtime_error_tests.cpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/19 18:17:56 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/19 18:38:45 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/Utils.hpp"
#include <gtest/gtest.h>

TEST(Utils_tests, runtime_error) {
    EXPECT_THROW({
            try {
            throw Utils::runtime_error("crash it all");
            } catch (Utils::runtime_error const& err) {
            EXPECT_STREQ("crash it all", err.what());
            throw ;
            }
            }, Utils::runtime_error);
}
