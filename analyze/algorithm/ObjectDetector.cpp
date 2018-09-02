#include "ObjectDetector.h"
#include <future>
#include <vector>
#include <algorithm>
#include "analyze/ThresholdDiffChecker.h"
#include <common/Context.hpp>
#include "ImagePair.h"

using namespace am::common::types;

namespace am {
	namespace analyze {
		namespace algorithm {

			ObjectDetector::ObjectDetector(const size_t threadsCount) :
				mThreadsCount(common::Context::getInstance()->getOpimalThreadsCount()),
				mPixelStep(5u), /// todo should be able to calibrate in runtime, resolution dependent constant
				mThreshold(20u) /// todo should be able to calibrate in runtime, user defined constant
			{
			}

			bool isNew(std::vector<Pixel>& object, Pixel newPos)
			{
				for (auto pos : object)
					if (pos.colId == newPos.colId && pos.rowId == newPos.rowId)
						return false;

				return true;
			}

			void pushCheckIfNew(std::vector<Pixel>& object, std::vector<Pixel>& toCheck, Pixel newPos)
			{
				if (isNew(object, newPos))
					toCheck.push_back(newPos);
			}

			//optimized dfs left/right borders for threads, every thread will search in defined area(column) of image
			std::vector<Pixel> dfs(Matrix<uint16_t>& changes, std::vector<Pixel>& toCheck, std::vector<Pixel>& object, size_t left, size_t right, size_t counter)
			{
				std::vector<Pixel> nextCheck;

				for (auto& position : toCheck)
				{
					if ((position.rowId >= 0 && position.colId >= 0 && position.rowId < changes.getHeight() &&
						position.colId < changes.getWidth()) &&
						position.colId >= left && position.colId <= right &&
						changes(position.rowId, position.colId) == common::CHANGE) /// todo check constant usage
					{
						pushCheckIfNew(object, nextCheck, Pixel{ position.rowId - 1, position.colId });
						pushCheckIfNew(object, nextCheck, Pixel{ position.rowId + 1, position.colId });
						pushCheckIfNew(object, nextCheck, Pixel{ position.rowId , position.colId - 1 });
						pushCheckIfNew(object, nextCheck, Pixel{ position.rowId , position.colId + 1 });

						Pixel newPos{ position.rowId, position.colId };

						if (isNew(object, newPos))
						{
							object.push_back(newPos);
							changes(position.rowId, position.colId) = counter;
						}
					}
				}
				if (nextCheck.size())
					dfs(changes, nextCheck, object, left, right, counter);

				return object;
			}

			std::vector<std::vector<Pixel>> startObjectsSearch(std::shared_ptr<Matrix<uint16_t>> changes, size_t step, size_t counter, size_t startPixel, size_t sectionWidth)
			{
				auto& chRef = *changes.get();

				std::vector<std::vector<Pixel>> resultList;
				// check all diffs on potential objects
				//if change found -> run DFS to figure out how many pixels included in this object
				for (size_t rowId = step; rowId < changes.get()->getHeight(); rowId += step)
				{
					for (size_t colId = startPixel; colId < sectionWidth; colId += step)
					{
						if (chRef(rowId, colId) == common::CHANGE)
						{
							std::vector<Pixel> obj = { Pixel{rowId, colId} };
							std::vector<Pixel> toCheck = { Pixel{ rowId - 1, colId },Pixel{ rowId + 1, colId },
								Pixel{ rowId, colId - 1 },Pixel{ rowId, colId + 1 } };

							resultList.push_back(dfs(chRef, toCheck, obj, startPixel, sectionWidth, counter++));
						}
					}
				}
				return resultList;
			}

			std::vector<Pixel> dfsInPair(ImagePair& pair, Matrix<uint16_t>& visited, std::vector<Pixel>& toCheck, std::vector<Pixel>& object, size_t left, size_t right)
			{
				std::vector<Pixel> nextCheck;

				for (auto& position : toCheck)
				{
					if ((position.rowId >= 0 && position.colId >= 0 && position.rowId < visited.getHeight() &&
						position.colId < visited.getWidth()) &&
						position.colId >= left && position.colId <= right &&
						visited(position.rowId, position.colId) != common::CHANGE &&
						pair.getAbsoluteDiff(position.rowId, position.colId) > 20)
					{

						visited(position.rowId, position.colId) = common::CHANGE;
						pushCheckIfNew(object, nextCheck, Pixel{ position.rowId - 1, position.colId });
						pushCheckIfNew(object, nextCheck, Pixel{ position.rowId + 1, position.colId });
						pushCheckIfNew(object, nextCheck, Pixel{ position.rowId , position.colId - 1 });
						pushCheckIfNew(object, nextCheck, Pixel{ position.rowId , position.colId + 1 });

						Pixel newPos{ position.rowId, position.colId };


						if (isNew(object, newPos))
						{
							object.push_back(newPos);
							visited(position.rowId, position.colId) = common::CHANGE;
						}
					}
				}
				if (nextCheck.size())
					dfsInPair(pair, visited, nextCheck, object, left, right);

				return object;
			}

			std::vector<std::vector<Pixel>> startObjectsSearchInPair(std::shared_ptr<ImagePair> pair, size_t step,  size_t startPixel, size_t sectionWidth)
			{
				//auto& chRef = *changes.get();
				auto& pairRef = *pair.get();
				Matrix<uint16_t> changes(pairRef.getWidth(), pairRef.getHeight());

				std::vector<std::vector<Pixel>> resultList;
				// check all diffs on potential objects
				//if change found -> run DFS to figure out how many pixels included in this object
				for (size_t rowId = step; rowId < pairRef.getHeight(); rowId += step)
				{
					for (size_t colId = startPixel; colId < sectionWidth; colId += step)
					{

						if (pairRef.getAbsoluteDiff(rowId, colId) > 20 && changes(rowId, colId) != common::CHANGE)
						{
							std::vector<Pixel> obj = { Pixel{ rowId, colId } };
							std::vector<Pixel> toCheck = { Pixel{ rowId - 1, colId },Pixel{ rowId + 1, colId },
														   Pixel{ rowId, colId - 1 },Pixel{ rowId, colId + 1 } };

							resultList.push_back(dfsInPair(pairRef, changes, toCheck, obj, startPixel, sectionWidth));
						}
					}
				}
				return resultList;
			}

			std::vector<Object> createObjectRects(std::vector<std::vector<std::vector<Pixel>>>& objPixels)
			{
				std::vector <std::vector<Object>> rects;

				for (auto thrList : objPixels)
				{
					std::vector<Object> threadObjs;
					for (auto objs : thrList)
					{
						if (objs.size() > 10) // objects with 5 pixels seems like noise - skipping(have to be confirmed)
							threadObjs.push_back(Object(objs));
					}
					rects.push_back(threadObjs);
				}

				std::vector<Object> res;

				if (rects.size() == 1)
					return *rects.begin();

				for (int leftId = 0; leftId < rects.size() - 1; ++leftId)
				{
					for (auto& leftItem : rects[leftId])
					{
						if (leftItem.getMaxWidth() != 0 && leftItem.getMaxHeight() != 0)
						{
							for (auto& rightItem : rects[leftId + 1])
							{
								leftItem.mergeIfPossible(rightItem);//) // if not merged add right separately
							}
							res.push_back(leftItem);
						}
					}
				}

				return res;
			}

			std::vector<Object> ObjectDetector::getObjectsRects(std::shared_ptr<Matrix<ColorChannelsDiff>> diffs)
			{
				const size_t width = diffs.get()->getWidth();
				const size_t height = diffs.get()->getHeight();
				size_t count = 0;
				const size_t columnWidth = width / mThreadsCount;

				std::shared_ptr<Matrix<uint16_t>> changes = ThresholdDiffChecker::getThresholdDiff(diffs, mThreadsCount, mThreshold);
				std::vector<std::vector<std::vector<Pixel>>> res;

				std::vector<std::future<std::vector<std::vector<Pixel>>>> futures;

				for (size_t columnId = 0; columnId <= mThreadsCount; ++columnId)
				{
					std::vector<Pixel> toCheck;
					futures.push_back(std::async(startObjectsSearch, changes, mPixelStep, count, columnId*columnWidth, columnId*columnWidth + columnWidth));
				}

				for (auto &e : futures)
					res.push_back(e.get());

				return createObjectRects(res);
			}

			std::vector<Object> ObjectDetector::getObjectsRects(std::shared_ptr<ImagePair> pair)
			{
				const size_t columnWidth = pair.get()->getWidth() / mThreadsCount;

				std::vector<std::vector<std::vector<Pixel>>> res;
				std::vector<std::future<std::vector<std::vector<Pixel>>>> futures;

				for (size_t columnId = 0; columnId <= mThreadsCount; ++columnId)
				{
					std::vector<Pixel> toCheck;
					futures.push_back(std::async(startObjectsSearchInPair, pair, mPixelStep, columnId*columnWidth, columnId*columnWidth + columnWidth));
				}

				for (auto &e : futures)
					res.push_back(e.get());

				return createObjectRects(res);
			}
		}
	}
}