#pragma once

#include <memory>
#include "common/Types.hpp"
#include "Object.h"


namespace recognition {
	namespace analyze {
		namespace algorithm {

			class ImagePair;

			class IObjectDetector
			{
			public:
				virtual ~IObjectDetector() = default;

				virtual std::vector<Object> getObjectsRects(std::shared_ptr<common::Matrix<common::ColorChannelsDiff>> diffs) = 0;
				virtual std::vector<Object> getObjectsRects(std::shared_ptr<ImagePair> pair) = 0;
			};
		}
	}
}