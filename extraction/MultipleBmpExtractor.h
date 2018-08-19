#pragma once

#include <vector>
#include <memory>
#include <common/Types.hpp>

namespace recognition
{
	// class for multiple reading files,
	//given vector of fieNames will returned with relative data from files
	class MultipleBmpExtractor
	{
	public:
		MultipleBmpExtractor() = default;
		virtual ~MultipleBmpExtractor() = default;

		std::vector<std::shared_ptr<common::Matrix<common::ColorChannels>>> readFiles(std::vector<std::string> fileNames);
	};
}
