#include "ve_pipeline.h"
#include <fstream>
#include <exception>
#include <iostream>
#include <cassert>


namespace ke {

	ve_pipeline::ve_pipeline(ve_device& device, const pipeline_config& info, const std::string& fd_vert, const std::string& fd_frag) : vedevice { device } {
		this->createGraphicsPipeline(fd_vert, fd_frag, info);
	}

	ve_pipeline::~ve_pipeline() {
		vkDestroyShaderModule(vedevice.device(), this->vertexShaderModule, nullptr);
		vkDestroyShaderModule(vedevice.device(), this->fragmentShaderModule, nullptr);
		vkDestroyPipeline(vedevice.device(), this->graphicsPipeline, nullptr);
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
		assert(info.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no pipeline layout provided in config info");
		assert(info.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no render pass  provided in config info");

		auto vertex_code   = readFile(fd_vert);
		auto fragment_code = readFile(fd_frag);

		// printf("Vertex shader: %llu bytes\nFragment shader: %llu bytes\n", vertex_code.size(), fragment_code.size()); // DEBUG
		createShaderModule(vertex_code, &vertexShaderModule);
		createShaderModule(fragment_code, &fragmentShaderModule);

		VkPipelineShaderStageCreateInfo shaderStages[2];
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertexShaderModule;
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr; 
		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragmentShaderModule;
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		// how to interpret initial input data
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;

		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = &info.viewport;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = &info.scissor;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState	 = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &info.inputAssemblyInfo;
		pipelineInfo.pRasterizationState = &info.rasterInfo;
		pipelineInfo.pColorBlendState	 = &info.colorBlendInfo;
		pipelineInfo.pMultisampleState	 = &info.multisampleInfo;
		pipelineInfo.pDepthStencilState  = &info.depthStencilInfo;
		pipelineInfo.pViewportState		 = &viewportInfo;
		pipelineInfo.pDynamicState		 = nullptr;
		pipelineInfo.layout				 = info.pipelineLayout;
		pipelineInfo.renderPass			 = info.renderPass;
		pipelineInfo.subpass			 = info.subpass;
		pipelineInfo.basePipelineIndex   = -1;
		pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(vedevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline");
		}
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


	// Stage 1) Input Assembly -- takes groups of vertices and groups them into geometry
	inline void ve_pipeline::createInputAssemblyStep(pipeline_config& configInfo, uint32_t width, uint32_t height) {
		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPrimitiveTopology.html
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
		configInfo.inputAssemblyInfo.pNext = nullptr;

		// viewport describes transformation between pipeline's output and target image
		configInfo.viewport.x = 0.0f;
		configInfo.viewport.y = 0.0f;
		configInfo.viewport.width = static_cast<float>(width);
		configInfo.viewport.height = static_cast<float>(height);
		configInfo.viewport.minDepth = 0.0f; // --these control the z component of shader's gl_Position
		configInfo.viewport.maxDepth = 1.0f; // ---

		// discard pixels outside of this rectangle
		configInfo.scissor.offset = { 0, 0 };
		configInfo.scissor.extent = { width, height };
	}

	// State 2) Rasterization -- break up geometry into fragments for where each pixel the triangle overlaps
	//https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPipelineRasterizationStateCreateInfo.html
	inline void ve_pipeline::createRasterizationStep(pipeline_config& configInfo) {
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

		// multisampling - how the rasterizer handles edges of geometry (anti-aliasing)
		configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		/*configInfo.multisampleInfo.minSampleShading = 1.0f;
		configInfo.multisampleInfo.pSampleMask = nullptr;
		configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;*/


	}

	inline void ve_pipeline::createColorBlendStep(pipeline_config& configInfo) {
		// how we combine colors in our frame buffer
		configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		configInfo.colorBlendInfo.attachmentCount = 1;
		configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
		//configInfo.colorBlendInfo.blendConstants[0] = 0.0f;
		//configInfo.colorBlendInfo.blendConstants[1] = 0.0f;
		//configInfo.colorBlendInfo.blendConstants[2] = 0.0f;
		//configInfo.colorBlendInfo.blendConstants[3] = 0.0f;

		configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
		//configInfo.depthStencilInfo.minDepthBounds = 0.0f;
		//configInfo.depthStencilInfo.maxDepthBounds = 1.0f;
		//configInfo.depthStencilInfo.front = {};
		//configInfo.depthStencilInfo.back = {};
	}

	pipeline_config ve_pipeline::defaultPipelineConfig(uint32_t width, uint32_t height) {
		pipeline_config configInfo{};
		createInputAssemblyStep(configInfo, width, height);
		createRasterizationStep(configInfo);		
		createColorBlendStep(configInfo);

		return configInfo;
	}
}