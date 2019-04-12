#pragma once 

#include"IComparer.h"

namespace am {
	namespace analyze {

		class ThresholdDiffChecker
		{
		public:
			ThresholdDiffChecker(const uint16_t channelTreshold);
			virtual ~ThresholdDiffChecker() = default;

			// shows persent of similarity based on diffs of two images
			float getAffinityPersent(const size_t threadsCount, std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>> diffs);

			// get matrix with values bigger then threshold
			static std::shared_ptr<common::types::Matrix<uint16_t>> getThresholdDiff(std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>> diffs,
				size_t threadsCount, size_t threshold);

		private:
			uint16_t mTreshold;
		};
	}
}