#include "../../Sys/src/BufReader.hpp"
#include "../../Sys/src/File.hpp"

#include <fcntl.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << argv[0] << " requires exactly one argument, a file to read" << std::endl;
        return 1;
    }

    File::Result file_res = File::open(argv[1], O_RDONLY, 0644);

    if (file_res.is_err()) {
        std::cerr << "Error: " << file_res.unwrap_err() << std::endl;
        return 1;
    }

    File file = file_res.unwrap();

    BufReader<File> reader = BufReader<File>::init(file);
    std::string line;

    while (!reader.eof()) {
        reader.read_line(line);
        std::cout << line;
        line.clear();
    }

    return 0;
}
