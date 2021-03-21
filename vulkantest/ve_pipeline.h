#pragma once
#include "ve_device.h"
#include <string>
#include <vector>

namespace ke {

	struct pipeline_config {
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class ve_pipeline
	{
	public:
		ve_pipeline(ve_device&, const pipeline_config&, const std::string&, const std::string&);
		~ve_pipeline();

		ve_pipeline(const ve_pipeline&) = delete;
		void operator=(const ve_pipeline&) = delete;

		static pipeline_config defaultPipelineConfig(uint32_t width, uint32_t height);

	private:
		static std::vector<char> readFile(const std::string&);
		void createGraphicsPipeline(const std::string& fd_vert, const std::string& fd_frag, const pipeline_config&);
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		static inline void createInputAssemblyStep(pipeline_config&, uint32_t, uint32_t);
		static inline void createRasterizationStep(pipeline_config&);
		static inline void createColorBlendStep(pipeline_config&);

		ve_device& vedevice; // possibly memory unsafe -- be careful
		VkPipeline graphicsPipeline;
		VkShaderModule vertexShaderModule;
		VkShaderModule fragmentShaderModule;
	};
}

