#include "ve_window.h"
#include <stdexcept>

namespace VULKENG {
	ve_window::ve_window(const int w, const int h, const std::string& name) : width{ w }, height{ h }, windowName{ name } {
		initWindow();
	}

	ve_window::~ve_window() {
		//vkDestroyInstance(instance, nullptr);
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}

	void ve_window::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		this->window = glfwCreateWindow(800, 600, this->windowName.c_str(), nullptr, nullptr);
	}

	void ve_window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, this->window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}
}