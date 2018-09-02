#pragma once

#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/ColorChannelsDiff.hpp"
#include "Object.h"

namespace am {
	namespace analyze {
		namespace algorithm {

			class ImagePair;

			class IObjectDetector
			{
			public:
				virtual ~IObjectDetector() = default;

				virtual std::vector<Object> getObjectsRects(std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>> diffs) = 0;
				virtual std::vector<Object> getObjectsRects(std::shared_ptr<ImagePair> pair) = 0;
			};
		}
	}
}