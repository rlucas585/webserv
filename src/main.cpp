#include "Slice/src/Slice.hpp"
#include <iostream>

int main(void) {
    Slice::Split iter("Project Compilation Successful");

    for (Slice slice = iter.next(); slice.isInitialized(); slice = iter.next()) {
        std::cout << slice << std::endl;
    }

    return 0;
}
