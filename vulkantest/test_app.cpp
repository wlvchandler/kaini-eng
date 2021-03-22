#include "test_app.h"

#include <stdexcept>
#include <array>

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
			drawFrame();
		}

		vkDeviceWaitIdle(veDevice.device());
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

	void test_app::createCommandBuffers() {
		commandBuffers.resize(veSwapchain.imageCount());
		VkCommandBufferAllocateInfo allocinfo{};
		allocinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocinfo.commandPool = veDevice.getCommandPool();
		allocinfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
		
		if (vkAllocateCommandBuffers(veDevice.device(), &allocinfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers");
		}

		for (int i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffers");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = veSwapchain.getRenderPass();
			renderPassInfo.framebuffer = veSwapchain.getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0,0 };
			renderPassInfo.renderArea.extent = veSwapchain.getSwapChainExtent();
			
			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			
			vePipeline->bind(commandBuffers[i]);
			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffers");
			}

		}

	}

	void test_app::drawFrame() {
		uint32_t image_index;
		auto result = veSwapchain.acquireNextImage(&image_index); // fetch frame we should render to next
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swapchain image"); // need to handle later, since can happen when window is resized
		}

		result = veSwapchain.submitCommandBuffers(&commandBuffers[image_index], &image_index);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}
	}
}