#pragma once
#include <string>
#include <vector>

namespace VULKENG {
	class ve_pipeline
	{
	public:
		ve_pipeline(const std::string&, const std::string&);
	private:
		static std::vector<char> readFile(const std::string&);
		void createGraphicsPipeline(const std::string&, const std::string&);
	};
}

