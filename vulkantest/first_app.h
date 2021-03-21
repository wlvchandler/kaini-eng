#pragma once

#include "ve_window.h"
#include "ve_pipeline.h"

namespace VULKENG {
	class first_app
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		void run();
	private:
		ve_window vulkWind{ WIDTH, HEIGHT, "Hello Vulkan!" };
		ve_pipeline vePipeline{ "shaders/sample_shader.vert.spv", "shaders/sample_shader.frag.spv" };
	};
}

