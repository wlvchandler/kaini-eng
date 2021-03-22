#include "test_app.h"

#include <stdexcept>

namespace ke {

	test_app::test_app() {
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	test_app::~test_app() {
		vkDestroyPipelineLayout(veDevice.device(), pipeline_layout, nullptr);
	}

	void test_app::run() {
		while (!this->veWindow.shouldClose()) {
			glfwPollEvents();
		}
	}

	void test_app::createPipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // to send small amount of data to shaders (not now since null)

		if (vkCreatePipelineLayout(veDevice.device(), &pipelineLayoutInfo, nullptr, &pipeline_layout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	void test_app::createPipeline() {
		auto pipelineConfig = ve_pipeline::defaultPipelineConfig(veSwapchain.width(), veSwapchain.height());
		pipelineConfig.renderPass = veSwapchain.getRenderPass();
		pipelineConfig.pipelineLayout = pipeline_layout;
		vePipeline = std::make_unique<ve_pipeline>(veDevice, pipelineConfig, "shaders/sample_shader.vert.spv", "shaders/sample_shader.frag.spv");
	}

	void test_app::createCommandBuffers() {}

	void test_app::drawFrame() {}
}