#include <iostream>
#include "test_module1/src/Formula.hpp"

int		main(void) {
	std::cout << "Project Compilation successful: " << std::endl;
	std::cout << "Bla: " << Formula::bla(2) << std::endl;

	return 0;
}
