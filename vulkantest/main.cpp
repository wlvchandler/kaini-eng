#include "test_app.h"
#include "ve_pipeline.h"

#include <iostream>
#include <cstdlib>
#include <exception>

int main() {

	ke::test_app FA{};
	try {
		FA.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}