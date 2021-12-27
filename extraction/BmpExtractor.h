#pragma once

#include "IExtractor.h"
#include "common/types/Matrix.hpp"
#include <memory>

namespace am
{
	namespace extraction
	{
		/// BmpExtractor class for RGB data extraction from BMP file
		class BmpExtractor
		{
		public:
			BmpExtractor() = default;
			virtual ~BmpExtractor() = default;

			static common::types::Matrix<common::types::Color24b> readFile(const std::string &filePath);
		};

	}
}
