#pragma once
#include "ve_engine_device.h"
#include <string>
#include <vector>

namespace ke {

	struct pipeline_config {};

	class ve_pipeline
	{
	public:
		ve_pipeline(const ve_engine_device&, const pipeline_config&, const std::string&, const std::string&);
	private:
		static std::vector<char> readFile(const std::string&);
		void createGraphicsPipeline(const std::string&, const std::string&);
	};
}

