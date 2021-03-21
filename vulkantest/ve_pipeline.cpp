#include "ve_pipeline.h"
#include <fstream>
#include <exception>
#include <iostream>


namespace ke {

	ve_pipeline::ve_pipeline(ve_device& device, const pipeline_config& info, const std::string& fd_vert, const std::string& fd_frag) : vedevice { device } {
		this->createGraphicsPipeline(fd_vert, fd_frag, info);
	}

	std::vector<char> ve_pipeline::readFile(const std::string& fp) {
		std::ifstream file{ fp, std::ios::ate | std::ios::binary };
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file: " + fp);
		}

		size_t n_bytes = static_cast<size_t>(file.tellg());
		std::vector<char> buf(n_bytes);

		file.seekg(0);
		file.read(buf.data(), n_bytes);
		file.close();

		return buf;
	}

	void ve_pipeline::createGraphicsPipeline(const std::string& fd_vert, const std::string& fd_frag, const pipeline_config& info) {
		auto vertex_code   = readFile(fd_vert);
		auto fragment_code = readFile(fd_frag);

		printf("Vertex shader: %llu bytes\nFragment shader: %llu bytes\n", vertex_code.size(), fragment_code.size());
	}

	void ve_pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		// http://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkShaderModuleCreateInfo.html
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO; // structure type
		createInfo.codeSize = code.size(); // must be a positive non-zero (multiple of 4 if SPIR-V)
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data()); // code used to create the shader module

		if (vkCreateShaderModule(vedevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module");
		}
	}

	pipeline_config ve_pipeline::defaultPipelineConfig(uint32_t width, uint32_t height) {
		pipeline_config configInfo{};
		return configInfo;
	}
}