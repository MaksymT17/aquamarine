#pragma once
#include"IObjectDetector.h"

namespace recognition {
	namespace analyze {
		namespace algorithm {
			
			class ObjectDetector : public IObjectDetector
			{
			public:
				ObjectDetector(const size_t threadsCount);
				virtual ~ObjectDetector() = default;

				virtual size_t getObjectsCount(std::shared_ptr<common::Matrix<common::ColorChannelsDiff>> diffs) override;

			private:
				size_t mThreadsCount;
			};
		}
	}
}