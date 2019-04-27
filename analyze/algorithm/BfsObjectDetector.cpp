#include "BfsObjectDetector.h"
#include <algorithm>
#include "analyze/ThresholdDiffChecker.h"
#include <common/Context.hpp>
#include "ImagePair.h"
#include <chrono>

namespace am {
	namespace analyze {
		namespace algorithm {
			using SharedColorDiffsMatrix = std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>>;
			using namespace am::common::types;
			using Pixels = std::vector<Pixel>;

			BfsObjectDetector::BfsObjectDetector(const size_t threads, std::shared_ptr<am::configuration::Configuration>& conf, std::shared_ptr<am::common::Logger>& logger)
				: mThreadsCount(threads)
				, mConfiguration(conf)
				, mLogger(new am::common::Logger())
			{
			}

			bool isNew(Pixels& object, Pixel newPos)
			{
				for (auto pos : object) 
				{
					if (pos.colId == newPos.colId && pos.rowId == newPos.rowId)
						return false;
				}
				return true;
			}

			void pushCheckIfNew(Pixels& object, Pixels& toCheck, Pixel newPos)
			{
				if (isNew(object, newPos))
					toCheck.push_back(newPos);
			}

			void checkClosest(Pixel& pos, Pixels& nextCheck, Pixels& object, Column col, const size_t& height, const size_t step)
			{
				if (pos.rowId - step >= 0)
					pushCheckIfNew(object, nextCheck, Pixel{ pos.rowId - step, pos.colId });
				if (pos.rowId + step < height)
					pushCheckIfNew(object, nextCheck, Pixel{ pos.rowId + step, pos.colId });
				if (pos.colId - step >= col.left)
					pushCheckIfNew(object, nextCheck, Pixel{ pos.rowId , pos.colId - step });
				if (pos.colId + step < col.left + col.right)
					pushCheckIfNew(object, nextCheck, Pixel{ pos.rowId , pos.colId + step });
			}

			Pixels checkConnections(const Pixel& px, const size_t& height, const Column& col, const size_t step)
			{
				Pixels toCheck;

				if (px.rowId - step >= 0)
					toCheck.push_back(Pixel{ px.rowId - step, px.colId });
				if (px.rowId + step < height)
					toCheck.push_back(Pixel{ px.rowId + step, px.colId });
				if (px.colId - step >= col.left)
					toCheck.push_back(Pixel{ px.rowId , px.colId - step });
				if (px.colId + step < col.left + col.right)
					toCheck.push_back(Pixel{ px.rowId , px.colId - step });

				return toCheck;
			}


			DescObjects BfsObjectDetector::createObjectRects(std::vector<std::vector<Pixels>>& objPixels)
			{
				std::vector <std::vector<Object>> rects;

				for (auto thrList : objPixels)
				{
					std::vector<Object> threadObjs;
					for (auto objs : thrList)
					{
						if (objs.size() > mConfiguration->MinPixelsForObject) // skip objects if smaller that minObjSize, avoid noise
							threadObjs.push_back(Object(objs));
					}
					rects.push_back(threadObjs);
				}

				DescObjects res;

				if (rects.size() == 1)
				{
					for (auto& r : *rects.begin())
						res.emplace(r);

					return res;
				}

				for (size_t leftId = 0; leftId < rects.size() - 1u; ++leftId)
				{
					for (auto& leftItem : rects[leftId])
					{
						if (leftItem.getMaxWidth() != 0 && leftItem.getMaxHeight() != 0)
						{
							for (auto& rightItem : rects[leftId + 1])
							{
								rightItem.mergeIfPossible(leftItem);
							}
							res.emplace(leftItem);
						}
					}
				}

				for (auto& rightItem : rects[rects.size() - 1u])
				{
					res.emplace(rightItem);
				}
				mLogger->info("BfsObjectDetector::%s, objects found:%d", __func__, res.size());
				return res;
			}

		}
	}
}
