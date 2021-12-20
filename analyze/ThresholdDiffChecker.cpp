#include "ThresholdDiffChecker.h"
#include <future>

namespace am {

	using namespace common::types;

	namespace analyze {

		ThresholdDiffChecker::ThresholdDiffChecker(const uint16_t channelTreshold) :
			mThreshold(channelTreshold) {}

		void setThresholdChanges(size_t rowId, size_t width, Matrix<Color24bDiff>& diff,
			const uint16_t threshold, MatrixU16& result)
		{
			for (std::size_t x = 0; x < width; ++x)
			{
				// if value bigger then trshoold we can assume - color changed
				if ((diff(rowId, x).r + diff(rowId, x).g + diff(rowId, x).b) > threshold)
					result(rowId, x) = common::CHANGE;
			}
		}

		void checlImageRow(size_t rowId, size_t width, Matrix<Color24bDiff>& diff,
			const uint16_t threshold, std::atomic_size_t& diffCounter)
		{
			for (std::size_t x = 0; x < width; ++x)
			{
				// if value bigger then threshold we can assume - color changed
				if ((diff(rowId, x).r + diff(rowId, x).g + diff(rowId, x).b) > threshold)
					++diffCounter;
			}
		}

		float ThresholdDiffChecker::getAffinityPersent(const size_t threadsCount, Matrix<Color24bDiff>& diffs)
		{
			std::atomic_size_t diffCounter(0);
			const size_t width = diffs.getWidth();
			const size_t height = diffs.getHeight();

			size_t threads = threadsCount > height ? height : threadsCount;

			std::vector<std::future<void>> futures;

			for (size_t portion = 0; portion < height; portion += threads)
			{
				for (size_t i = 0; i < threads; ++i)
					futures.emplace_back(std::async(std::launch::async, checlImageRow, 
					portion + i, width, std::ref(diffs), mThreshold, std::ref(diffCounter)));

				futures.clear();
			}
			// final section in case if width not divided normally on threads count
			for (size_t lastLines = (height / threads) * threads; lastLines < height; ++lastLines)
				futures.emplace_back(std::async(std::launch::async, checlImageRow, lastLines, width, 
				std::ref(diffs), mThreshold, std::ref(diffCounter)));

			for (auto &e : futures)
				e.get();

			return 1.0f - (static_cast<float>(diffCounter) / static_cast<float>(width * height));
		}

		MatrixU16 ThresholdDiffChecker::getThresholdDiff(Matrix<Color24bDiff>& diffs, size_t threadsCount, size_t threshold)
		{
			const size_t width = diffs.getWidth();
			const size_t height = diffs.getHeight();
			MatrixU16 res(width, height);
			std::vector<std::future<void>> futures;

			for (size_t portion = 0; portion < height; portion += threadsCount) {
				for (size_t i = 0; i < threadsCount; ++i)
					futures.emplace_back(std::async(std::launch::async, setThresholdChanges, portion + i, width, std::ref(diffs), threshold, std::ref(res)));

				for (auto &e : futures)
					e.get();
			}

			// final section in case if width not divided normally on threads count
			for (size_t lastLines = (height / threadsCount) * threadsCount; lastLines < height; ++lastLines)
				futures.emplace_back(std::async(std::launch::async, setThresholdChanges, lastLines, width, std::ref(diffs), threshold, std::ref(res)));

			return res;
		}

	} // namespace analyze
} // namespace am
