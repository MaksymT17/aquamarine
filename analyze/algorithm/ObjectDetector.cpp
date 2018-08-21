#include "ObjectDetector.h"

namespace recognition {
	namespace analyze {
		namespace algorithm {

			ObjectDetector::ObjectDetector(const size_t threadsCount):
				mThreadsCount(threadsCount)
			{
			}

			void doDfsSearch() {}

			size_t ObjectDetector::getObjectsCount(std::shared_ptr<common::Matrix<common::ColorChannelsDiff>> diffs)
			{
				size_t count = 0;

				return count;
			}
			
		}
	}
}