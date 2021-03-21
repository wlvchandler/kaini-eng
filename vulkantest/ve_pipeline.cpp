#include "ve_pipeline.h"
#include <fstream>
#include <exception>
#include <iostream>


namespace ke {

	ve_pipeline::ve_pipeline(const std::string& fd_vert, const std::string& fd_frag) {
		this->createGraphicsPipeline(fd_vert, fd_frag);
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

	void ve_pipeline::createGraphicsPipeline(const std::string& fd_vert, const std::string& fd_frag) {
		auto vertex_code   = readFile(fd_vert);
		auto fragment_code = readFile(fd_frag);

		printf("Vertex shader: %llu bytes\nFragment shader: %llu bytes\n", vertex_code.size(), fragment_code.size());
	}
}