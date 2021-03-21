#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace ke {
	class ve_window
	{
	public:
		ve_window(int w, int h, const std::string& name);
		~ve_window();
		
		ve_window(const ve_window&) = delete;
		ve_window& operator=(const ve_window&) = delete;

		inline bool shouldClose() { return glfwWindowShouldClose(window); }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void initWindow();
		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;
	};
}

