#include <gtest/gtest.h>

#include "../src/Utils.hpp"

TEST(Time, current_time) { std::cout << Utils::Time::now().to_http_string() << std::endl; }
