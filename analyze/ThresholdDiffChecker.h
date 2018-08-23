
#include"IComparer.h"

namespace recognition {
	namespace analyze {

		class ThresholdDiffChecker
		{
		public:
			ThresholdDiffChecker(const uint16_t channelTreshold);
			virtual ~ThresholdDiffChecker() = default;

			float getAffinityPersent(std::shared_ptr<common::Matrix<common::ColorChannelsDiff>> diffs);
			static std::shared_ptr<common::Matrix<uint16_t>> getThresholdDiff(std::shared_ptr<common::Matrix<common::ColorChannelsDiff>> diffs, size_t threadsCount, uint16_t threshold);

		private:
			uint16_t mTreshold;
		};
	}
}