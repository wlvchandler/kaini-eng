#pragma once
#include "ve_device.h"
#include <string>
#include <vector>

namespace ke {

	struct pipeline_config {};

	class ve_pipeline
	{
	public:
		ve_pipeline(ve_device&, const pipeline_config&, const std::string&, const std::string&);
		~ve_pipeline() {};

		ve_pipeline(const ve_pipeline&) = delete;
		void operator=(const ve_pipeline&) = delete;

		static pipeline_config defaultPipelineConfig(uint32_t width, uint32_t height);

	private:
		static std::vector<char> readFile(const std::string&);
		void createGraphicsPipeline(const std::string& fd_vert, const std::string& fd_frag, const pipeline_config&);
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		ve_device& vedevice; // memory unsafe -- be careful
		VkPipeline graphicsPipeline;
		VkShaderModule vertexShaderModule;
		VkShaderModule fragmentShaderModule;
	};
}

