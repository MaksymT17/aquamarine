
#include"IComparer.h"

namespace recognition {
	namespace analyze {

		class TresholdDiffChecker
		{
		public:
			TresholdDiffChecker(const uint16_t channelTreshold);
			virtual ~TresholdDiffChecker() = default;

			float getAffinityPersent(std::shared_ptr<common::Matrix<common::ColorChannelsDiff>> diffs);

			uint16_t mTreshold;
		};
	}
}