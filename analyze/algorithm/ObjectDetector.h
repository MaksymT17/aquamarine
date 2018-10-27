#pragma once
#include "IObjectDetector.h"
#include <cstdlib>
#include "configuration/Configuration.hpp"
#include "common/Logger.hpp"

namespace am {

	namespace analyze {
		namespace algorithm {

			class ImagePair;
			using SharedColorDiffsMatrix = std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>>;

			// class for object detection, extracted data should be reusable for next iterations
			// return value of detection is vector of Objects - Rectangled area
			class ObjectDetector : public IObjectDetector
			{
			public:
				
				ObjectDetector();
				virtual ~ObjectDetector() = default;

				//all calculation with diffs, no time limits
				virtual std::vector<Object> getObjectsRects(SharedColorDiffsMatrix diffs) override;
				
				//fast search with time limitations
				virtual std::vector<Object> getObjectsRects(std::shared_ptr<ImagePair> pair) override;

			private:
				size_t mThreadsCount;
				am::configuration::Configuration mConfiguration;
				std::shared_ptr<am::common::Logger> mLogger;
			};
		}
	}
}