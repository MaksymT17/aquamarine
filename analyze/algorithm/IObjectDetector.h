#pragma once

#include<memory>
#include"common/Types.hpp"

namespace recognition {
	namespace analyze {
		namespace algorithm {
			
			class IObjectDetector
			{

			public:
				virtual ~IObjectDetector() = default;

				virtual size_t getObjectsCount(std::shared_ptr<common::Matrix<common::ColorChannelsDiff>> diffs) = 0;
			};
		}
	}
}