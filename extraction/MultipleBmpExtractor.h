#pragma once

#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/Color24b.hpp"
#include "common/Logger.hpp"
#include "IMultipleBmpExtractor.h"

namespace am::extraction
{
	// class for multiple reading files, given vector of fieNames will be
	// fullfilled as return value with relative data from files
	// Usage of async calls can reduce extraction time.
	class MultipleBmpExtractor : public IMultipleBmpExtractor
	{
	public:
		explicit MultipleBmpExtractor(std::shared_ptr<am::common::Logger> &logger);
		~MultipleBmpExtractor() = default;

		// fill up the Matrices for each file provided in the input parameter
		virtual std::vector<common::types::Matrix<common::types::Color24b>> readFiles(std::vector<std::string> &&fileNames) override;

	private:
		std::shared_ptr<am::common::Logger> mLogger;
	};
} // namespace am::extraction
