#pragma once
#include "IObjectDetector.h"
#include <cstdlib>
#include "configuration/Configuration.hpp"


namespace am {

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

				//all calculation with diffs, no time limits
				virtual std::vector<Object> getObjectsRects(std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>> diffs) override;
				
				//fast search with time limitations
				virtual std::vector<Object> getObjectsRects(std::shared_ptr<ImagePair> pair) override;

			private:
				size_t mThreadsCount;
				am::configuration::Configuration mConfiguration;
			};
		}
	}
}