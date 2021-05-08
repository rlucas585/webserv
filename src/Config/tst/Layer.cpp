#include "gtest/gtest.h"

#include "../src/Layer.hpp"

class LayerTester : public ::testing::Test {
  public:
    Layer root_layer = Layer::init("http", 0, 0);

    void SetUp(void) override {
        Layer::PushResult first_server_res = root_layer.push_layer("server");
        if (first_server_res.is_ok()) {
            Layer& first_server = *(first_server_res.unwrap());

            first_server.add_value("key", "value");
            first_server.add_value("time", "place");
        }

        Layer::PushResult second_server_res = root_layer.push_layer("server");
        if (second_server_res.is_ok()) {
            Layer& second_server = *(second_server_res.unwrap());

            second_server.add_value("time", "place");
            second_server.add_value("somewhere", "beyond");
        }
    }
};

TEST_F(LayerTester, initialization) {
    std::cout << root_layer;
    EXPECT_EQ(root_layer.to_string(), "http {\n"
                                      "\n"
                                      "  server {\n"
                                      "    key value;\n"
                                      "    time place;\n"
                                      "\n"
                                      "  }\n"
                                      "  server {\n"
                                      "    somewhere beyond;\n"
                                      "    time place;\n"
                                      "\n"
                                      "  }\n"
                                      "}\n");
}
