#pragma once

#include "ve_window.h"
#include "ve_model.h"
#include "ve_pipeline.h"
#include "ve_device.h"
#include "ve_swapchain.h"

#include <memory>
#include <vector>

namespace ke {
	class test_app
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

	public:
		test_app();
		~test_app();

		test_app(const test_app&) = delete;
		test_app& operator=(const test_app&) = delete;

		void run();
	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

	private:
		ve_window veWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		ve_device veDevice{ veWindow };
		ve_swapchain veSwapchain{ veDevice, veWindow.getExtent() };
		std::unique_ptr<ve_pipeline> vePipeline;
		VkPipelineLayout pipeline_layout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<ve_model> veModel;
	};
}

