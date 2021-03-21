#pragma once

#include "ve_window.h"
#include "ve_pipeline.h"
#include "ve_device.h"

namespace ke {
	class first_app
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		void run();
	private:
		ve_window veWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		ve_device veDevice{ veWindow };
		ve_pipeline vePipeline{ veDevice, ve_pipeline::defaultPipelineConfig(WIDTH, HEIGHT), "shaders/sample_shader.vert.spv", "shaders/sample_shader.frag.spv" };
	};
}

