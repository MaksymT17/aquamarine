#pragma once

#include "common/types/Matrix.hpp"
#include "common/types/Color24b.hpp"
#include <string>

namespace am::extraction
{
	/// JpgExtractor class for RGB data extraction from JPG/JPEG file
	class JpgExtractor
	{
	public:
		JpgExtractor() = default;
		~JpgExtractor() = default;

		static common::types::Matrix<common::types::Color24b> readFile(const std::string &filePath);
	};

}