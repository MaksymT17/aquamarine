#pragma once

#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/Color24b.hpp"
#include "IMultipleExtractor.h"

#include "ExtractorFactory.h"

namespace am::extraction
{
	// class for multiple reading files, given vector of fieNames will be
	// fullfilled as return value with relative data from files
	// Usage of async calls can reduce extraction time.
	class MultipleExtractor : public IMultipleExtractor
	{

	public:
		MultipleExtractor(std::unique_ptr<ExtractorFactory> factory);
		~MultipleExtractor() = default;

		// fill up the Matrices for each file provided in the input parameter
		virtual std::vector<common::types::Matrix<common::types::Color24b>> readFiles(std::vector<std::string> &&fileNames) override;
		
	private:
		std::unique_ptr<ExtractorFactory> m_factory;
	};
}
