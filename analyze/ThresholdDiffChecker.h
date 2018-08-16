
#include"IComparer.h"

namespace recognition {
	namespace analyze {

		class ThresholdDiffChecker
		{
		public:
			ThresholdDiffChecker(const uint16_t channelTreshold);
			virtual ~ThresholdDiffChecker() = default;

			float getAffinityPersent(std::shared_ptr<common::Matrix<common::ColorChannelsDiff>> diffs);

			uint16_t mTreshold;
		};
	}
}