// Test if C++98 can use the optional class

#include "src/optional.hpp"
#include <iostream>

int main(void) {
    Utils::optional<int> opt(5);

    std::cout << opt.value() << std::endl;
    return 0;
}
