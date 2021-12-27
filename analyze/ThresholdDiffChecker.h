#pragma once

#include "IComparer.h"

namespace am
{
	namespace analyze
	{

		class ThresholdDiffChecker
		{
		public:
			ThresholdDiffChecker(const uint16_t channelTreshold);
			virtual ~ThresholdDiffChecker() = default;

			// shows persent of similarity based on diffs of two images
			float getAffinityPersent(const size_t threadsCount, common::types::Matrix<common::types::Color24bDiff> &diffs);

			// get matrix with values bigger then threshold
			static common::types::MatrixU16 getThresholdDiff(common::types::Matrix<common::types::Color24bDiff> &diffs,
															 size_t threadsCount, size_t threshold);

		private:
			uint16_t mThreshold;
		};
	}
}