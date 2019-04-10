#pragma once

#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/ColorChannelsDiff.hpp"
#include "Object.h"

namespace am {
	namespace analyze {
		namespace algorithm {

			class ImagePair;

			// interface for Object properties extraction
			// pair of images is a minimal requirement for collecting objects
			class IObjectDetector
			{
			public:
				virtual ~IObjectDetector() = default;

				virtual DescObjects getObjectsRects(std::shared_ptr<ImagePair> pair) = 0;
			};
		}
	}
}