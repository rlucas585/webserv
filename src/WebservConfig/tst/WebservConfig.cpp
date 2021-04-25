#include "WebservConfig.hpp"
#include <cstdio>
#include <gtest/gtest.h>

using namespace Webserv;

TEST(WebservConfig_tests, basic_parsing) {
	{
		ConfigStore cf_store;
		ConfigStore::ParseResult res = cf_store.parse_configfile("./nginx.conf.sample1");
		EXPECT_TRUE(res.is_ok());
	}
}
