#include "AffinityComparer.h"
#include "common/Constants.hpp"
#include "common/exceptions/AllocationException.hpp"
#include <future>
#include "common/exceptions/AmException.hpp"

using namespace am::common::types;

namespace am {
	namespace analyze {


		void fillPixelLineWithDiffs(std::shared_ptr<Matrix<Color24b>> base, std::shared_ptr<Matrix<Color24b>> toCompare,
			std::shared_ptr<Matrix<Color24bDiff>> result, size_t rowId, size_t width) {

			for (size_t i = 0; i < width; ++i) {
				const auto &baseRef = *base.get();
				const auto &toCompareRef = *toCompare.get();
				auto &diffResult = *result.get();

				diffResult(rowId, i) = Color24bDiff(baseRef(rowId, i), toCompareRef(rowId, i));
			}
		}

		AffinityComparer::AffinityComparer(std::shared_ptr<Matrix<Color24b>> base)
			: mBase(base),
			mMode(DataMode::KEEP_BASE_FRAME)
		{}

		bool validateComparingRequestedSize(size_t sourceW, size_t sourceH, size_t targetW, size_t targetH)
		{
			return (sourceW == targetW) && (sourceH == targetH);
		}

		bool AffinityComparer::isRequestSizeValid(std::shared_ptr<Matrix<Color24b>> newSource) const
		{
			const auto & base = mBase.get();
			const auto & compare = newSource.get();
			return (base->getWidth() == compare->getWidth() && base->getHeight() == compare->getHeight());
		}

		//deprecated
		std::shared_ptr<Matrix<Color24bDiff>> AffinityComparer::compare(std::shared_ptr<Matrix<Color24b>> newSource) {
			const size_t width = mBase.get()->getWidth();
			const size_t height = mBase.get()->getHeight();
			//common::Context::getInstance()->logging().logInfo("AffinityComparer::compare width:%zd height:%zd.", width, height);

			if (!validateComparingRequestedSize(width, height, newSource.get()->getWidth(), newSource.get()->getHeight()))
			{
				std::string msg("Size of base and compared image are different, comparison not supported.");
				throw common::exceptions::AllocationException(msg);
			}
			// can be wrapped in try() for bad alloc
			std::shared_ptr<Matrix<Color24bDiff>> result(new Matrix<Color24bDiff>(width, height));

			size_t availableThrCount = am::common::getOptimalThreadsCount();
			size_t threadsCount = availableThrCount > height ? height : availableThrCount;

			std::vector<std::future<void>> futures;

			for (size_t portion = 0; portion < height; portion += threadsCount) {
				for (size_t i = 0; i < threadsCount; ++i)
					futures.emplace_back(std::async(std::launch::async, fillPixelLineWithDiffs, mBase, newSource, result, portion + i, width));

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

		std::shared_ptr<Matrix<Color24bDiff>> AffinityComparer::compare(std::shared_ptr<Matrix<Color24b>> first, 
			std::shared_ptr<Matrix<Color24b>> second, size_t threadsNum)
		{
			const size_t width = first.get()->getWidth();
			const size_t height = first.get()->getHeight();
			if (width != second.get()->getWidth() || height != second.get()->getHeight()) {
				std::string msg("AffinityComparer size of images are different.");
				throw am::common::exceptions::AmException(msg);
			}

			std::shared_ptr<Matrix<Color24bDiff>> result(new Matrix<Color24bDiff>(width, height));

			size_t threadsCount = threadsNum > height ? height : threadsNum;

			std::vector<std::future<void>> futures;

			for (size_t portion = 0; portion < height; portion += threadsCount) {
				for (size_t i = 0; i < threadsCount; ++i)
					futures.push_back(std::async(std::launch::async, fillPixelLineWithDiffs, first, second, result, portion + i, width));

				futures.clear();
			}

			// final section in case if width not divided normally on threads count
			for (size_t lastLines = (height / threadsCount) * threadsCount; lastLines < height; ++lastLines)
				futures.push_back(std::async(fillPixelLineWithDiffs, first, second, result, lastLines, width));

			for (auto &e : futures)
				e.get();

			return result;

		}
	} // namespace analyze
} // namespace am
