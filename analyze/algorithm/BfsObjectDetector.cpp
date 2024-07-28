#include "BfsObjectDetector.h"
#include "ImagePair.h"
#include "analyze/ThresholdDiffChecker.h"
#include <algorithm>
#include <chrono>
#include <common/Context.hpp>

namespace am::analyze::algorithm
{
	using SharedColorDiffsMatrix =
		std::shared_ptr<common::types::Matrix<common::types::Color24bDiff>>;
	using namespace am::common::types;
	using Pixels = std::vector<Pixel>;

	bool isNew(const Pixels &object, size_t rowId, size_t colId) noexcept
	{
		return !std::all_of(object.cbegin(), object.cend(), [&](auto pos)
							{ return (pos.colId != colId && pos.rowId != rowId); });
	}

	void pushCheckIfNew(const Pixels &object, Pixels &toCheck, size_t rowId,
						size_t colId)
	{
		if (isNew(object, rowId, colId))
		{
			toCheck.emplace_back(rowId, colId);
		}
	}

	void checkClosest(size_t rowId, size_t colId, Pixels &nextCheck, ObjectRectangle &object,
					  ImageRowSegment &row, const size_t width, const size_t step) noexcept
	{
		if (static_cast<int>(rowId - step) >= static_cast<int>(row.start))
		{
			// mv: commented in sake of optimization
			// pushCheckIfNew(object, nextCheck, rowId - step, colId);
			nextCheck.emplace_back(rowId - step, colId);
		}
		if (rowId + step < row.end)
		{
			// pushCheckIfNew(object, nextCheck, rowId + step, colId);
			nextCheck.emplace_back(rowId + step, colId);
		}
		if (static_cast<int>(colId - step) >= 0)
		{
			// pushCheckIfNew(object, nextCheck, rowId, colId - step);
			nextCheck.emplace_back(rowId, colId - step);
		}
		if (colId + step < width)
		{
			// pushCheckIfNew(object, nextCheck, rowId, colId + step);
			nextCheck.emplace_back(rowId, colId + step);
		}
	}

	Pixels checkConnections(size_t rowId, size_t colId, const size_t &width,
							const ImageRowSegment &row, const size_t step) noexcept
	{
		Pixels toCheck;

		if (static_cast<int>(rowId - step) >= static_cast<int>(row.start))
		{
			toCheck.emplace_back(rowId - step, colId);
		}
		if (rowId + step < static_cast<int>(row.end))
		{
			toCheck.emplace_back(rowId + step, colId);
		}
		if (static_cast<int>(colId - step) >= 0)
		{
			toCheck.emplace_back(rowId, colId - step);
		}
		if (colId + step < width)
		{
			toCheck.emplace_back(rowId, colId + step);
		}

		return toCheck;
	}

	BfsObjectDetector::BfsObjectDetector(
		const size_t threads,
		const am::configuration::Configuration &conf,
		std::shared_ptr<am::common::Logger> &logger)
		: mThreadsCount(threads), mConfiguration(conf), mLogger(logger) {}

	DescObjects BfsObjectDetector::createObjectRects(
		std::vector<std::vector<ObjectRectangle>> &objPixels, const size_t minPixels)
	{
		std::vector<std::vector<ObjectRectangle>> rects;
		rects.resize(objPixels.size());

		DescObjects orderedResult;
		for (auto &thrList : objPixels)
		{
			if (!thrList.empty())
			{
				std::vector<ObjectRectangle> threadObjs;

				std::copy(thrList.begin(), thrList.end(), std::back_inserter(threadObjs));

				rects.push_back(threadObjs);
			}
		}

		if (rects.size() == 1u)
		{
			for (auto &r : *rects.begin())
			{
				orderedResult.emplace(r);
			}

			return orderedResult;
		}

		for (size_t leftId = 0u; leftId < rects.size() - 1u; ++leftId)
		{
			for (auto &leftItem : rects[leftId])
			{
				bool isMerged = false;
				for (auto &rightItem : rects[leftId + 1])
				{
					if (rightItem.mergeIfPossibleVerticalToMe(leftItem))
					{
						isMerged = true;
						leftItem.clearPixelsCount();
					}
				}

				if (!isMerged && leftItem.getPixelsCount() >= minPixels)
					orderedResult.emplace(leftItem);
			}
		}
		// last thread results already merged if possible
		for (auto &rightItem : rects[rects.size() - 1])
		{
			if (rightItem.getPixelsCount() >= minPixels)
				orderedResult.emplace(rightItem);
		}

		mLogger->info("BfsObjectDetector::%s, objects found:%d", __func__,
					  orderedResult.size());

		return orderedResult;
	}

} // namespace namespace am::analyze::algorithm
