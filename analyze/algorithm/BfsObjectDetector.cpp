#include "BfsObjectDetector.h"
#include "ImagePair.h"
#include "analyze/ThresholdDiffChecker.h"
#include <algorithm>
#include <chrono>
#include <common/Context.hpp>

namespace am {
	namespace analyze {
		namespace algorithm {
			using SharedColorDiffsMatrix =
				std::shared_ptr<common::types::Matrix<common::types::Color24bDiff>>;
			using namespace am::common::types;
			using Pixels = std::vector<Pixel>;

			bool isNew(Pixels &object, size_t rowId, size_t colId)noexcept
			{
				for (auto pos : object) {
					if (pos.colId == colId && pos.rowId == rowId)
						return false;
				}
				return true;
			}

			void pushCheckIfNew(Pixels &object, Pixels &toCheck, size_t rowId,
				size_t colId)
			{
				if (isNew(object, rowId, colId)) {
					toCheck.emplace_back(rowId, colId);
				}
			}

			void checkClosest(size_t rowId, size_t colId, Pixels &nextCheck, Pixels &object,
				Column col, const size_t height, const size_t step)noexcept
			{
				if (static_cast<int>(rowId - step) >= 0) {
					pushCheckIfNew(object, nextCheck, rowId - step, colId);
				}
				if (rowId + step < height) {
					pushCheckIfNew(object, nextCheck, rowId + step, colId);
				}
				if (static_cast<int>(colId - step) >= static_cast<int>(col.left)) {
					pushCheckIfNew(object, nextCheck, rowId, colId - step);
				}
				if (colId + step < col.right) {
					pushCheckIfNew(object, nextCheck, rowId, colId + step);
				}
			}

			Pixels checkConnections(size_t rowId, size_t colId, const size_t &height,
				const Column &col, const size_t step) noexcept
			{
				Pixels toCheck;

				if (static_cast<int>(rowId - step) >= 0) {
					toCheck.emplace_back(rowId - step, colId);
				}
				if (rowId + step < height) {
					toCheck.emplace_back(rowId + step, colId);
				}
				if (static_cast<int>(colId - step) >= (int)col.left) {
					toCheck.emplace_back(rowId, colId - step);
				}
				if (colId + step < col.right) {
					toCheck.emplace_back(rowId, colId + step);
				}

				return toCheck;
			}

			BfsObjectDetector::BfsObjectDetector(
				const size_t threads,
				std::shared_ptr<am::configuration::Configuration> &conf,
				std::shared_ptr<am::common::Logger> &logger)
				: mThreadsCount(threads), mConfiguration(conf), mLogger(logger) {}

			DescObjects BfsObjectDetector::createObjectRects(
				std::vector<std::vector<Pixels>> &objPixels, const size_t minPixels)
			{
				std::vector<std::vector<Object>> rects;
				DescObjects orderedResult;
				for (auto &thrList : objPixels) {
					if (!thrList.empty()) {
						std::vector<Object> threadObjs;
						for (auto& objs : thrList) {
							// skip objects if smaller that minObjSize, avoid noise
							//if (objs.size() >= mConfiguration->MinPixelsForObject)
							threadObjs.emplace_back(objs);
						}

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
							if (rightItem.mergeIfPossibleLeftToMe(leftItem))
							{
								isMerged = true;
								leftItem.clearPixelsCount();
							}
						}

						if (!isMerged && leftItem.getPixelsCount() >= minPixels)
							orderedResult.emplace(leftItem);
					}
				}
				//last thread results already merged if possible
				for (auto &rightItem : rects[rects.size() - 1])
				{
					if (rightItem.getPixelsCount() >= minPixels)
						orderedResult.emplace(rightItem);
				}

				mLogger->info("BfsObjectDetector::%s, objects found:%d", __func__,
					orderedResult.size());

				return orderedResult;
			}

		} // namespace algorithm
	} // namespace analyze
} // namespace am
