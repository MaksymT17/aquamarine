

#include"TresholdDiffChecker.h"
#include <future>

using namespace common;

namespace recognition {
	namespace analyze {

		TresholdDiffChecker::TresholdDiffChecker(const uint16_t channelTreshold) :
			mTreshold(channelTreshold) {}

		void isDiffInsideOfTreshold(uint16_t treshold, size_t& counter, const ColorChannelsDiff& chDiff)
		{
			if ((chDiff.r + chDiff.g + chDiff.b) < treshold)
				++counter;
		}

		float TresholdDiffChecker::getAffinityPersent(std::shared_ptr<Matrix<ColorChannelsDiff>> diffs)
		{
			float channelDiffCounter = 0;

			const size_t width = diffs.get()->getWidth();
			const size_t height = diffs.get()->getHeight();

			auto &diffRef = *diffs.get();

			for (std::size_t y = 0; y < height; ++y)
			{
				for (std::size_t x = 0; x < width; ++x)
				{
					//printf("chcked %d %d \n", y,x);
					// if value bigger then trshoold we can assume - color changed
					if ((diffRef(y, x).r + diffRef(y, x).g + diffRef(y, x).b) > mTreshold)
						++channelDiffCounter;
				}
			}

			return 1.0f - (channelDiffCounter / static_cast<float>(width * height));
		}

	} // namespace analyze
} // namespace recognition
