#include "gtest/gtest.h"

#include "ServerLogic.hpp"

TEST(ServerLogic, error_page_bad_request) {
    std::string output_string;

    ServerLogic::serve_error_page(http::State(http::BadRequest_400), output_string);
    EXPECT_EQ(output_string, "<html>\n"
                             "<head><title>400 Bad Request</title></head>\n"
                             "<body>\n"
                             "<center><h1>400 Bad Request</h1></center>\n"
                             "<hr><center>rlucas + sverschu WebServ</center>\n"
                             "</body>\n"
                             "</html>\n\n");
}

TEST(ServerLogic, error_page_length_required) {
    std::string output_string;

    ServerLogic::serve_error_page(http::State(http::LengthRequired_411), output_string);
    EXPECT_EQ(output_string, "<html>\n"
                             "<head><title>411 Length Required</title></head>\n"
                             "<body>\n"
                             "<center><h1>411 Length Required</h1></center>\n"
                             "<hr><center>rlucas + sverschu WebServ</center>\n"
                             "</body>\n"
                             "</html>\n\n");
}
