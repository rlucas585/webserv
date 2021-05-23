#include "gtest/gtest.h"

#include "../src/Layer.hpp"
#include "../src/Parser.hpp"

class LayerTester : public ::testing::Test {
  public:
    Layer root_layer = Layer::init("http", 0, 0);

    void SetUp(void) override {
        Layer::Result first_server_res = root_layer.push_layer("server");
        if (first_server_res.is_ok()) {
            Layer& first_server = *(first_server_res.unwrap());

            first_server.add_value("key", "value");
            first_server.add_value("time", "place");
        }

        Layer::Result second_server_res = root_layer.push_layer("server");
        if (second_server_res.is_ok()) {
            Layer& second_server = *(second_server_res.unwrap());

            second_server.add_value("time", "place");
            second_server.add_value("somewhere", "beyond");

            Layer::Result second_server_location_result = second_server.push_layer("location");
            if (second_server_location_result.is_ok()) {
                Layer& location = *(second_server_location_result.unwrap());

                location.add_value("more", "values");
                location.add_value("further", "beyond");
            }
        }

        Layer::Result random_block_res = root_layer.push_layer("random");
        if (random_block_res.is_ok()) {
            Layer& random_block = *(random_block_res.unwrap());

            random_block.add_value("fire", "moltres");
            random_block.add_value("electric", "zapdos");
            random_block.add_value("ice", "articuno");
        }
    }
};

// Test iteration over values filtered by an identifier - and access of map
// values from within a Layer
TEST_F(LayerTester, iteration) {
    size_t count = 0;

    Layer::Iterator filtered = root_layer.filter_children("server");
    for (; filtered != root_layer.end_children(); filtered++) {
        if (count == 0) {
            EXPECT_EQ(*filtered->get_value("key").value(), "value");
            EXPECT_EQ(*filtered->get_value("time").value(), "place");
            EXPECT_FALSE(filtered->get_value("somewhere").has_value());
        }
        if (count == 1) {
            EXPECT_EQ(*filtered->get_value("time").value(), "place");
            EXPECT_EQ(*filtered->get_value("somewhere").value(), "beyond");
            EXPECT_FALSE(filtered->get_value("key").has_value());
        }
        count += 1;
    }

    Layer::Iterator filtered2 = root_layer.filter_children("random");
    for (; filtered2 != root_layer.end_children(); filtered2++) {
        EXPECT_EQ(*filtered2->get_value("electric").value(), "zapdos");
        EXPECT_EQ(*filtered2->get_value("fire").value(), "moltres");
        EXPECT_EQ(*filtered2->get_value("ice").value(), "articuno");
        EXPECT_FALSE(filtered2->get_value("key").has_value());
    }
}

// Layer::Location values can be used to reach any point of the data structure
// from any other. (Pointers may have been sufficient for this project however)
TEST_F(LayerTester, Location) {
    std::vector<Layer::Location> locations;

    Layer::Iterator block = root_layer.begin_children();
    for (; block != root_layer.end_children(); block++) {
        locations.push_back(block->get_location());
    }
    EXPECT_EQ(root_layer.get_layer(locations[0]).unwrap()->get_name(), "server");
    EXPECT_EQ(root_layer.get_layer(locations[1]).unwrap()->get_name(), "server");
    EXPECT_EQ(root_layer.get_layer(locations[2]).unwrap()->get_name(), "random");
}

TEST(Layer, basic_parsing) {
    config::Parser parser;

    config::Parser::Result result = parser.generate_config_from_file("config.txt");

    if (result.is_err()) {
        std::cout << result.unwrap_err() << std::endl;
    }
    ASSERT_FALSE(result.is_err());
    Layer config = result.unwrap();

    // Repeated Iterative loops such as this should not be present in production code
    for (Layer::Iterator block = config.begin_children(); block != config.end_children(); block++) {
        if (block->get_name() == "http") {
            for (Layer::Iterator server = block->filter_children("server");
                 server != block->end_children(); server++) {
                for (Layer::Iterator server_child = server->filter_children("limit_except");
                     server_child != server->end_children(); server_child++) {
                    EXPECT_EQ(*server_child->get_value("allow").value(), "192.168.1.0/32");
                    EXPECT_EQ(*server_child->get_value("deny").value(), "all");
                }
            }
        }
    }
}

// TEST(Layer, complex_parsing) {
//     config::Parser parser;
//
//     config::Parser::Result result = parser.generate_config_from_file("nginx_example.txt");
//
//     if (result.is_err()) {
//         std::cout << result.unwrap_err() << std::endl;
//     }
//     ASSERT_FALSE(result.is_err());
//     Layer config = result.unwrap();
//     std::cout << config;
// }
