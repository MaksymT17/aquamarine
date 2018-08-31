#include "AffinityComparer.h"
#include "common/Context.hpp"
#include "common/Types.hpp"
#include "common/AllocationException.hpp"
#include <future>

using namespace std;
using namespace common::types;

namespace aq {
	namespace analyze {

		ColorChannelsDiff getChannelsDiff(const ColorChannels &source, const ColorChannels &toCompare) {
			ColorChannelsDiff diff;
			uint8_t positives = 0u; // details: see Types.hpp about bitmask usage

			setUpChannelDiff(source.r, toCompare.r, diff.r, positives, common::R_POS_VAL);
			setUpChannelDiff(source.g, toCompare.g, diff.g, positives, common::G_POS_VAL);
			setUpChannelDiff(source.b, toCompare.b, diff.b, positives, common::B_POS_VAL);

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
			{
				std::string msg("Size of base and compared image are different, comparison not supported.");
				throw common::AllocationException(msg);
			}
			// can be wrapped in try() for bad alloc
			shared_ptr<Matrix<ColorChannelsDiff>> result(new Matrix<ColorChannelsDiff>(width, height));

			size_t availableThrCount = common::Context::getInstance()->getOpimalThreadsCount();
			size_t threadsCount = availableThrCount > height ? height : availableThrCount;

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
} // namespace aq
