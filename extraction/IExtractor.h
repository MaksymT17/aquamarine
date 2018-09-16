
#pragma once

#include <string>
#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/ColorChannels.hpp"

namespace am
{
	namespace extraction
	{
		class IExtractor
		{
		public:
			virtual ~IExtractor() = default;

			virtual std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> readFile(std::string& filePath) = 0;
		};
	}
}
