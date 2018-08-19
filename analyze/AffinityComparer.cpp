#include "AffinityComparer.h"
#include <future>

using namespace std;
using namespace common;

namespace recognition {
	namespace analyze {

		ColorChannelsDiff getChannelsDiff(const ColorChannels &source, const ColorChannels &toCompare) {
			ColorChannelsDiff diff;
			uint8_t positives = 0u; // details: see Types.hpp about bitmask usage

			setUpChannelDiff(source.r, toCompare.r, diff.r, positives, R_POS_VAL);
			setUpChannelDiff(source.g, toCompare.g, diff.g, positives, G_POS_VAL);
			setUpChannelDiff(source.b, toCompare.b, diff.b, positives, B_POS_VAL);

			diff.positives = positives;
			return diff;
		}

		void fillPixelLineWithDiffs(shared_ptr<Matrix<ColorChannels>> base, shared_ptr<Matrix<ColorChannels>> toCompare,
			shared_ptr<Matrix<ColorChannelsDiff>> result, size_t rowId, size_t width) {

			for (size_t i = 0; i < width; ++i) {
				const auto &baseRef = *base.get();
				const auto &toCompareRef = *toCompare.get();
				auto &diffResult = *result.get();

				diffResult(rowId, i) = getChannelsDiff(baseRef(rowId, i), toCompareRef(rowId, i));
			}
		}

		size_t getOptimalThreadsCount(size_t rowsLimit)
		{
			const float MAX_CPU_LOAD_PERCENT = 0.7f;
			// 70% of CPU threads for our stuff, others - for maintaining background proccess

			unsigned int MAX_THREADS = std::thread::hardware_concurrency();
			const float concurrentCallsCount = static_cast<float>(MAX_THREADS) * MAX_CPU_LOAD_PERCENT;
			size_t threadsCount = concurrentCallsCount < 1.0f ? 1 : static_cast<size_t>(concurrentCallsCount);

			return threadsCount > rowsLimit ? rowsLimit : threadsCount;
		}

		AffinityComparer::AffinityComparer(shared_ptr<Matrix<ColorChannels>> base)
			: mBase(base),
			mMode(DataMode::KEEP_BASE_FRAME)
		{}

		bool validateComparingRequestedSize(size_t sourceW, size_t sourceH, size_t targetW, size_t targetH)
		{
			return (sourceW == targetW) && (sourceH == targetH);
		}

		bool AffinityComparer::isRequestSizeValid(shared_ptr<Matrix<ColorChannels>> newSource) const
		{
			const auto & base = mBase.get();
			const auto & compare = newSource.get();
			return (base->getWidth() == compare->getWidth() && base->getHeight() == compare->getHeight());
		}

		shared_ptr<Matrix<ColorChannelsDiff>> AffinityComparer::compare(shared_ptr<Matrix<ColorChannels>> newSource) {
			const size_t width = mBase.get()->getWidth();
			const size_t height = mBase.get()->getHeight();

			if (!validateComparingRequestedSize(width, height, newSource.get()->getWidth(), newSource.get()->getHeight()))
				return nullptr;
			// can be wrapped in try() for bad alloc
			shared_ptr<Matrix<ColorChannelsDiff>> result(new Matrix<ColorChannelsDiff>(width, height));

			size_t threadsCount = getOptimalThreadsCount(height);

			std::vector<std::future<void>> futures;

			for (size_t portion = 0; portion < height; portion += threadsCount) {

				for (size_t i = 0; i < threadsCount; ++i)
					futures.push_back(std::async(fillPixelLineWithDiffs, mBase, newSource, result, portion + i, width));

				for (auto &e : futures)
					e.get();

				futures.clear();
			}

			
			// final section in case if width not divided normally on threads count
			for (size_t lastLines = (height / threadsCount) * threadsCount; lastLines < height; ++lastLines)
				futures.push_back(std::async(fillPixelLineWithDiffs, mBase, newSource, result, lastLines, width));

			for (auto &e : futures)
				e.get();

			if (mMode == SWITCH_TO_COMPARED)
				mBase = newSource;

			return result;
		}
	} // namespace analyze
} // namespace recognition
