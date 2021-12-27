
#pragma once

#include <string>
#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/Color24b.hpp"

namespace am
{
	namespace extraction
	{
		class IExtractor
		{
		public:
			virtual ~IExtractor() = default;

			virtual common::types::Matrix<common::types::Color24b> readFile(std::string &filePath) = 0;
		};
	}
}
