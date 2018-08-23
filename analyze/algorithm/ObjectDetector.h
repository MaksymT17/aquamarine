#pragma once
#include"IObjectDetector.h"

namespace recognition {
	namespace analyze {
		namespace algorithm {
			// class for object detection, extracted data should be reusable for next iterations
			//Ahtung! Have to be reviwed, and optimized as well!
			//return value of detection is vector of positions

			class ObjectDetector : public IObjectDetector
			{
			public:
				ObjectDetector(const size_t threadsCount);
				virtual ~ObjectDetector() = default;

				virtual std::vector<std::vector<Position>> getObjectsCount(std::shared_ptr<common::Matrix<common::ColorChannelsDiff>> diffs) override;

			private:
				size_t mThreadsCount;
				size_t mPixelStep;
				uint8_t mThreshold;
			};
		}
	}
}