#include <iostream>
#include "FileDesc.hpp"

#include <fcntl.h>
#include <unistd.h>

int main(void) {
    {
        int         fd = open("output.txt", O_WRONLY | O_TRUNC, 0644);
        FileDesc    file = FileDesc::init(fd);

        file.writeToFile("hello there");
    }
    {
        int         fd = open("output.txt", O_RDONLY, 0644);
        FileDesc    file = FileDesc::init(fd);
        std::string str;

        std::cout << "'" << str << "' size: " << str.size() << std::endl;
        file.readFromFile(str, 5);
        std::cout << "'" << str << "' size: " << str.size() << std::endl;
        file.readFromFile(str, 10);
        std::cout << "'" << str << "' size: " << str.size() << std::endl;
    }

    return 0;
}
