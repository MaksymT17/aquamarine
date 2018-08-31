#pragma once
#include"IObjectDetector.h"
#include <cstdlib>

namespace aq {
	namespace analyze {
		namespace algorithm {
			class ImagePair;
			// class for object detection, extracted data should be reusable for next iterations
			//return value of detection is vector of Objects - Rectangled area

			class ObjectDetector : public IObjectDetector
			{
			public:
				ObjectDetector(const size_t threadsCount);
				virtual ~ObjectDetector() = default;

				virtual std::vector<Object> getObjectsRects(std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>> diffs) override;
				virtual std::vector<Object> getObjectsRects(std::shared_ptr<ImagePair> pair) override;

			private:
				size_t mThreadsCount;
				size_t mPixelStep;
				uint8_t mThreshold;
			};
		}
	}
}