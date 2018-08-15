
#pragma once

#include <string>
#include "../common/Types.hpp"

namespace recognition
{
	class IExtractor
	{
	public:
		virtual ~IExtractor() = default;

		virtual std::shared_ptr<common::Matrix<common::ColorChannels>> readFile(std::string& filePath) = 0;

	};
}
