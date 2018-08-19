#include"ThresholdDiffChecker.h"
#include <future>
#include "common/Context.hpp"

using namespace common;

namespace recognition {
	namespace analyze {

		ThresholdDiffChecker::ThresholdDiffChecker(const uint16_t channelTreshold) :
			mTreshold(channelTreshold) {}

		void checlImageRow(size_t rowId, size_t width, std::shared_ptr<Matrix<ColorChannelsDiff>> diff, const uint16_t threshold, std::shared_ptr<std::atomic_size_t> diffCounter)
		{
			auto &diffRef = *diff.get();
			for (std::size_t x = 0; x < width; ++x)
			{
				// if value bigger then trshoold we can assume - color changed
				if ((diffRef(rowId, x).r + diffRef(rowId, x).g + diffRef(rowId, x).b) > threshold)
					++(*diffCounter.get());
			}
		}

		float ThresholdDiffChecker::getAffinityPersent(std::shared_ptr<Matrix<ColorChannelsDiff>> diffs)
		{
			std::shared_ptr<std::atomic_size_t> diffCounter(new std::atomic_size_t(0));

			const size_t width = diffs.get()->getWidth();
			const size_t height = diffs.get()->getHeight();

			size_t availableThrCount = common::Context::getInstance()->getOpimalThreadsCount();
			size_t threadsCount = availableThrCount > height ? height : availableThrCount;

			std::vector<std::future<void>> futures;

			for (size_t portion = 0; portion < height; portion += threadsCount) {
				for (size_t i = 0; i < threadsCount; ++i)
					futures.push_back(std::async(checlImageRow, portion + i, width, diffs, mTreshold, diffCounter));

				for (auto &e : futures)
					e.get();

				futures.clear();
			}
			
			// final section in case if width not divided normally on threads count
			for (size_t lastLines = (height / threadsCount) * threadsCount; lastLines < height; ++lastLines)
				futures.push_back(std::async(checlImageRow, lastLines, width, diffs, mTreshold, diffCounter));

			for (auto &e : futures)
				e.get();

			return 1.0f - (static_cast<float>(*diffCounter.get()) / static_cast<float>(width * height));
		}

	} // namespace analyze
} // namespace recognition
