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

		// Stage 1) Input Assembly -- takes groups of vertices and groups them into geometry
		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPrimitiveTopology.html
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
		configInfo.inputAssemblyInfo.pNext = nullptr;

		// viewport describes transformation between pipeline's output and target image
		configInfo.viewport.x = 0.0f;
		configInfo.viewport.y = 0.0f;
		configInfo.viewport.width  = static_cast<float>(width);
		configInfo.viewport.height = static_cast<float>(height);
		configInfo.viewport.minDepth = 0.0f; // --these control the z component of shader's gl_Position
		configInfo.viewport.maxDepth = 1.0f; // ---

		// discard pixels outside of this rectangle
		configInfo.scissor.offset = { 0, 0 };
		configInfo.scissor.extent = { width, height };

		configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.viewportInfo.viewportCount = 1;
		configInfo.viewportInfo.pViewports = &configInfo.viewport;
		configInfo.viewportInfo.scissorCount = 1;
		configInfo.viewportInfo.pScissors = &configInfo.scissor;

		// State 2) Rasterization -- break up geometry into fragments for where each pixel the triangle overlaps
		//https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPipelineRasterizationStateCreateInfo.html
		configInfo.rasterInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.rasterInfo.depthClampEnable = VK_FALSE;			// do not clep fragment's depth values (https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#fragops-depth)
		configInfo.rasterInfo.rasterizerDiscardEnable = VK_FALSE;	// do not discard primitives immediately before rasterization
		configInfo.rasterInfo.polygonMode = VK_POLYGON_MODE_FILL;   // render polygons using rasterization rules here
		configInfo.rasterInfo.lineWidth = 1.0f;
		configInfo.rasterInfo.cullMode = VK_CULL_MODE_NONE;			// do not discard any triangles...yet (BACK_BIT for backface culling)
		configInfo.rasterInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;  // triangle with negative area is considered front-facing
		configInfo.rasterInfo.depthBiasEnable = VK_FALSE;
		configInfo.rasterInfo.depthBiasConstantFactor = 0.0f;
		configInfo.rasterInfo.depthBiasClamp = 0.0f;
		configInfo.rasterInfo.depthBiasSlopeFactor = 0.0f;



		return configInfo;
	}
}