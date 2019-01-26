
#include"IComparer.h"

namespace am {
	namespace analyze {

		class ThresholdDiffChecker
		{
		public:
			ThresholdDiffChecker(const uint16_t channelTreshold);
			virtual ~ThresholdDiffChecker() = default;

			float getAffinityPersent(std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>> diffs);
			static std::shared_ptr<common::types::Matrix<uint16_t>> getThresholdDiff(std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>> diffs, size_t threadsCount, size_t threshold);

		private:
			uint16_t mTreshold;
		};
	}
}