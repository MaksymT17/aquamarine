
#pragma once

#include <string>
#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/Color24b.hpp"

namespace am::extraction
{
	class IMultipleBmpExtractor
	{
	public:
		~IMultipleBmpExtractor() = default;

		virtual std::vector<common::types::Matrix<common::types::Color24b>> readFiles(std::vector<std::string> &&fileNames) = 0;
	};
} // namespace am::extraction
