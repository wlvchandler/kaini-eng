#include "first_app.h"


namespace ke {
	void first_app::run() {
		while (!this->vulkWind.shouldClose()) {
			glfwPollEvents();
		}
	}
}