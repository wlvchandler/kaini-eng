#include "first_app.h"


namespace VULKENG {
	void first_app::run() {
		while (!this->vulkWind.shouldClose()) {
			glfwPollEvents();
		}
	}
}