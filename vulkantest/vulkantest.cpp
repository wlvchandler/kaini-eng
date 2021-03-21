//#pragma warning(disable : 26812) // disable C26812 on windows -- regarding scoped enums
//
//#define GLFW_INCLUDE_VULKAN
//#include <GLFW\glfw3.h>
//
//#include <iostream>
//#include <stdexcept>
//#include <cstdlib>
//
//struct HelloTriangle {
//	GLFWwindow* window;
//	VkInstance instance;
//
//
//	void initWindow() {
//		glfwInit();
//		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//
//		window = glfwCreateWindow(800, 600, "my window", nullptr, nullptr);
//	}
//
//	void mainloop() {
//		while (!glfwWindowShouldClose(window)) {
//			glfwPollEvents();
//		}
//	}
//
//	void cleanup() {
//		vkDestroyInstance(instance, nullptr);
//		glfwDestroyWindow(window);
//		glfwTerminate();
//	}
//
//	void createInstance() {
//		VkApplicationInfo appInfo{};
//		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//		appInfo.pApplicationName = "Hello Triangle";
//		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//		appInfo.pEngineName = "No Engine";
//		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//		appInfo.apiVersion = VK_API_VERSION_1_0;
//
//		VkInstanceCreateInfo createInfo{};
//		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//		createInfo.pApplicationInfo = &appInfo;
//
//		// basically tell vulkan driver we're on windows
//		uint32_t glfwExtensionCount = 0;
//		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//		createInfo.enabledExtensionCount = glfwExtensionCount;
//		createInfo.ppEnabledExtensionNames = glfwExtensions;
//
//		createInfo.enabledLayerCount = 0; // no global validation layers enabled yet
//
//		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
//			throw std::runtime_error("failed to create instance");
//		}
//	}
//
//	void initVulkan() {
//		createInstance();
//	}
//
//	void run() {
//		initVulkan();
//		initWindow();
//		mainloop();
//		cleanup();
//	}
//};
//
//int main() {
//	HelloTriangle ht = HelloTriangle();
//	ht.run();
//	return 0;
//}