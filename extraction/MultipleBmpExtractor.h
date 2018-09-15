#pragma once

#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/ColorChannels.hpp"
#include "common/Logger.hpp"
namespace am
{
	namespace extraction
	{
		// class for multiple reading files,
		//given vector of fieNames will returned with relative data from files
		class MultipleBmpExtractor
		{
		public:
			MultipleBmpExtractor();
			virtual ~MultipleBmpExtractor() = default;
			std::vector<std::shared_ptr<common::types::Matrix<common::types::ColorChannels>>> readFiles(std::vector<std::string> fileNames);

		private:
			std::shared_ptr<am::common::Logger> mLogger;
		};
	}
}
