#pragma once

#include "IExtractor.h"
#include "common/types/ColorChannels.hpp"
#include "common/types/Matrix.hpp"
#include <memory>


namespace am
{
	namespace extraction
	{
		///todo: implement. purpose of this class - use thirdparties library to extract all pixels information
		/// and fill required Matrix with all data, bitmap reader supports at least BMP format images :)
		class BmpExtractor
		{
		public:
			BmpExtractor() = default;
			virtual ~BmpExtractor() = default;

			static std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> readFile(std::string& filePath);
			
		};
	}
}
