#pragma once

#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/Color24b.hpp"
#include "common/Logger.hpp"
#include "IMultipleExtractor.h"

namespace am::extraction
{
	// class for multiple reading files, given vector of fieNames will be
	// fullfilled as return value with relative data from files
	// Usage of async calls can reduce extraction time.
	class MultipleExtractor : public IMultipleExtractor
	{

	public:
		MultipleExtractor(std::shared_ptr<am::common::Logger> &logger, bool is_bmp = false);
		~MultipleExtractor() = default;

		// fill up the Matrices for each file provided in the input parameter
		virtual std::vector<common::types::Matrix<common::types::Color24b>> readFiles(std::vector<std::string> &&fileNames) override;

	private:
		std::shared_ptr<am::common::Logger> mLogger;
		bool mIsForBmp;
	};
}
