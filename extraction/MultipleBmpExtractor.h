#pragma once

#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/ColorChannels.hpp"

namespace aq
{
	namespace extraction
	{
		// class for multiple reading files,
		//given vector of fieNames will returned with relative data from files
		class MultipleBmpExtractor
		{
		public:
			MultipleBmpExtractor() = default;
			virtual ~MultipleBmpExtractor() = default;

			std::vector<std::shared_ptr<common::types::Matrix<common::types::ColorChannels>>> readFiles(std::vector<std::string> fileNames);
		};
	}
}
