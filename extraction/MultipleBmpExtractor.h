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
		// Usage of async calls can reduce extraction time.
		class MultipleBmpExtractor
		{
		public:
			MultipleBmpExtractor(std::shared_ptr<am::common::Logger> &logger);
			virtual ~MultipleBmpExtractor() = default;

			// fill up the Matrices for each file provided in the input parameter
			std::vector<common::types::Matrix<common::types::Color24b>> readFiles(std::vector<std::string> &fileNames);

		private:
			std::shared_ptr<am::common::Logger> mLogger;
		};
	}
}
