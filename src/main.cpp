#include "Str/src/Str.hpp"
#include <iostream>

int main(void) {
    Str::Split iter("Project Compilation Successful");

    for (Str slice = iter.next(); slice.isInitialized(); slice = iter.next()) {
        std::cout << slice << std::endl;
    }

    return 0;
}
