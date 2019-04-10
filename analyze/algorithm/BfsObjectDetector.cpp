#include "BfsObjectDetector.h"
#include <future>
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
					if (pos.colId == newPos.colId && pos.rowId == newPos.rowId)
						return false;

				return true;
			}

			void pushCheckIfNew(Pixels& object, Pixels& toCheck, Pixel newPos)
			{
				if (isNew(object, newPos))
					toCheck.push_back(newPos);
			}

			void checkClosest(Pixel& pos, Pixels& nextCheck, Pixels& object, Column col, const size_t& height)
			{
				if (pos.rowId - 1 >= 0)
					pushCheckIfNew(object, nextCheck, Pixel{ pos.rowId - 1, pos.colId });
				if (pos.rowId + 1 < height)
					pushCheckIfNew(object, nextCheck, Pixel{ pos.rowId + 1, pos.colId });
				if (pos.colId - 1 >= col.left)
					pushCheckIfNew(object, nextCheck, Pixel{ pos.rowId , pos.colId - 1 });
				if (pos.colId + 1 < col.left + col.right)
					pushCheckIfNew(object, nextCheck, Pixel{ pos.rowId , pos.colId + 1 });
			}

			Pixels checkConnections(const Pixel& px, const size_t& height, const Column& col)
			{
				Pixels toCheck;

				if (px.rowId - 1 >= 0)
					toCheck.push_back(Pixel{ px.rowId - 1, px.colId });
				if (px.rowId + 1 < height)
					toCheck.push_back(Pixel{ px.rowId + 1, px.colId });
				if (px.colId - 1 >= col.left)
					toCheck.push_back(Pixel{ px.rowId , px.colId - 1 });
				if (px.colId + 1 < col.left + col.right)
					toCheck.push_back(Pixel{ px.rowId , px.colId - 1 });

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

				for (size_t leftId = 0; leftId < rects.size() - 1u; ++leftId) {
					for (auto& leftItem : rects[leftId])
					{
						if (leftItem.getMaxWidth() != 0 && leftItem.getMaxHeight() != 0)
						{
							for (auto& rightItem : rects[leftId + 1])
							{
								leftItem.mergeIfPossible(rightItem);
							}
							res.emplace(leftItem);
						}
					}
				}

				return res;
			}
			
		}
	}
}
