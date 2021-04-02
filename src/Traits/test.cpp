// Test if C++98 can use the type_traits header

#include "src/type_traits.hpp"
#include <iostream>

int main(void) {
    typedef typename meta::type_with_alignment<meta::alignment_of<int>::value>::type aligner;

    std::cout << "hello world" << std::endl;
    std::cout << sizeof(aligner) << std::endl;

    return 0;
}
