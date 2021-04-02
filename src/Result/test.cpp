// Test if C++98 can use the result class

#include "src/result.hpp"
#include "../Utils/src/Utils.hpp"
#include <iostream>

int main(void) {
    Utils::result<int, std::string> opt(5);

    std::cout << opt.ok() << std::endl;
    return 0;
}
