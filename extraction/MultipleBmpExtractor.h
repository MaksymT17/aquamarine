#pragma once

#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/Color24b.hpp"
#include "common/Logger.hpp"
namespace am
{
	namespace extraction
	{
		// class for multiple reading files, given vector of fieNames will be
		// fullfilled as return value with relative data from files
		class MultipleBmpExtractor
		{
		public:
			MultipleBmpExtractor(std::shared_ptr<am::common::Logger>& logger);
			virtual ~MultipleBmpExtractor() = default;
			std::vector<std::shared_ptr<common::types::Matrix<common::types::Color24b>>> readFiles(std::vector<std::string> fileNames);

		private:
			std::shared_ptr<am::common::Logger> mLogger;
		};
	}
}
