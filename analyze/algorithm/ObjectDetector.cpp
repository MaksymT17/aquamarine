#include "ObjectDetector.h"
#include <future>
#include <algorithm>
#include "analyze/ThresholdDiffChecker.h"
#include <common/Context.hpp>
#include "ImagePair.h"
#include <chrono>

namespace am {
	namespace analyze {
		namespace algorithm {

			using namespace am::common::types;
			using Pixels = std::vector<Pixel>;

			ObjectDetector::ObjectDetector()
				: mThreadsCount(common::Context::getInstance()->getOpimalThreadsCount())
				, mConfiguration(common::Context::getInstance()->getConfiguration())
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

			//optimized bfs depending to left/right borders for threads, 
			//every thread will search in defined area(column) of image
			Pixels bsf(Matrix<uint16_t>& changes, Pixels& toCheck, Pixels& object, Column col)
			{
				std::vector<Pixel> nextCheck;

				for (auto& position : toCheck)
				{
					if (changes(position.rowId, position.colId) == common::CHANGE)
					{
						if (position.rowId - 1 >= 0)
							pushCheckIfNew(object, nextCheck, Pixel{ position.rowId - 1, position.colId });
						if (position.rowId + 1 < changes.getHeight())
							pushCheckIfNew(object, nextCheck, Pixel{ position.rowId + 1, position.colId });
						if (position.colId - 1 >= col.left)
							pushCheckIfNew(object, nextCheck, Pixel{ position.rowId , position.colId - 1 });
						if (position.colId + 1 < col.left + col.right)
							pushCheckIfNew(object, nextCheck, Pixel{ position.rowId , position.colId + 1 });

						Pixel newPos{ position.rowId, position.colId };

						if (isNew(object, newPos))
						{
							object.push_back(newPos);
							changes(position.rowId, position.colId) = 0;
						}
					}
				}
				if (nextCheck.size())
					bsf(changes, nextCheck, object, col);

				return object;
			}

			std::vector<Pixels> startObjectsSearch(std::shared_ptr<Matrix<uint16_t>> changes, size_t step, Column col)
			{
				auto& chRef = *changes.get();

				std::vector<Pixels> resultList;
				// check all diffs on potential objects
				//if change found -> run bsf to figure out how many pixels included in this object
				for (size_t rowId = step; rowId < chRef.getHeight(); rowId += step)
				{
					for (size_t colId = col.left; colId < col.right; colId += step)
					{
						if (chRef(rowId, colId) == common::CHANGE)
						{
							Pixels obj = { Pixel{rowId, colId} };
							Pixels toCheck;

							if (rowId - 1 >= 0)
								toCheck.push_back(Pixel{ rowId - 1, colId });
							if (rowId + 1 < chRef.getHeight())
								toCheck.push_back(Pixel{ rowId + 1, colId });
							if (colId - 1 >= col.left)
								toCheck.push_back(Pixel{ rowId , colId - 1 });
							if (colId + 1 < col.left + col.right)
								toCheck.push_back(Pixel{ rowId , colId - 1 });

							resultList.push_back(bsf(chRef, toCheck, obj, col));
						}
					}
				}
				return resultList;
			}

			std::vector<Pixel> bsfInPair(ImagePair& pair, Matrix<uint16_t>& visited, Pixels& toCheck, Pixels& object,
				Column col, std::chrono::steady_clock::time_point& startTime, const configuration::Configuration& conf)
			{
				auto timeNow = std::chrono::steady_clock::now();
				std::chrono::duration<double> calcDuration = timeNow - startTime;
				if (calcDuration.count() >= conf.CalculationTimeLimit)
				{
					///todo: make Error notification about failed detection
					printf("Timelimit for calculation exceded. So much noise in picture.\n");
					return object;
				}

				Pixels nextCheck;

				for (auto& position : toCheck)
				{
					if (visited(position.rowId, position.colId) != common::CHANGE && 
						pair.getAbsoluteDiff(position.rowId, position.colId) > conf.AffinityThreshold)
					{
						visited(position.rowId, position.colId) = common::CHANGE;

						if (position.rowId - 1 >= 0)
							pushCheckIfNew(object, nextCheck, Pixel{ position.rowId - 1, position.colId });
						if (position.rowId + 1 < visited.getHeight())
							pushCheckIfNew(object, nextCheck, Pixel{ position.rowId + 1, position.colId });
						if (position.colId - 1 >= col.left)
							pushCheckIfNew(object, nextCheck, Pixel{ position.rowId , position.colId - 1 });
						if (position.colId + 1 < col.right)
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
					bsfInPair(pair, visited, nextCheck, object, col, startTime, conf);

				return object;
			}

			std::vector<std::vector<Pixel>> startObjectsSearchInPair(std::shared_ptr<ImagePair> pair, 
				size_t step, Column col, const configuration::Configuration conf)
			{
				auto startTime = std::chrono::steady_clock::now();
				auto& pairRef = *pair.get();
				Matrix<uint16_t> changes(pairRef.getWidth(), pairRef.getHeight());

				std::vector<Pixels> resultList;
				// check all diffs on potential objects
				//if change found -> run bsf to figure out how many pixels included in this object
				for (size_t rowId = step; rowId < pairRef.getHeight(); rowId += step)
				{
					for (size_t colId = col.left; colId < col.right; colId += step)
					{
						auto timeNow = std::chrono::steady_clock::now();
						std::chrono::duration<double> calcDuration = timeNow - startTime;
						if (calcDuration.count() >= conf.CalculationTimeLimit)
						{
							///todo: make Error notification about failed detection
							printf("Timelimit for calculation exceded. So much noise in picture.\n");
							return resultList;
						}
						else if (pairRef.getAbsoluteDiff(rowId, colId) > conf.AffinityThreshold &&
							changes(rowId, colId) != common::CHANGE)
						{
							Pixels obj = { Pixel{rowId, colId} };
							Pixels toCheck;

							if (rowId - 1 >= 0)
								toCheck.push_back(Pixel{ rowId - 1, colId });
							if (rowId + 1 <= pairRef.getHeight())
								toCheck.push_back(Pixel{ rowId + 1, colId });
							if (colId - 1 >= col.left)
								toCheck.push_back(Pixel{ rowId , colId - 1 });
							if (colId + 1 < col.right)
								toCheck.push_back(Pixel{ rowId , colId - 1 });
							
							resultList.push_back(bsfInPair(pairRef, changes, toCheck, obj, col, startTime, conf));
						}
					}
				}
				return resultList;
			}

			std::vector<Object> createObjectRects(std::vector<std::vector<Pixels>>& objPixels, const size_t minObjSize)
			{
				std::vector <std::vector<Object>> rects;

				for (auto thrList : objPixels)
				{
					std::vector<Object> threadObjs;
					for (auto objs : thrList)
					{
						if (objs.size() > minObjSize) // skip objects if smaller that minObjSize, avoid noise
							threadObjs.push_back(Object(objs));
					}
					rects.push_back(threadObjs);
				}

				std::vector<Object> res;

				if (rects.size() == 1)
					return *rects.begin();

				for (size_t leftId = 0; leftId < rects.size() - 1u; ++leftId)
				{
					for (auto& leftItem : rects[leftId])
					{
						if (leftItem.getMaxWidth() != 0 && leftItem.getMaxHeight() != 0)
						{
							for (auto& rightItem : rects[leftId + 1])
							{
								leftItem.mergeIfPossible(rightItem);
							}
							res.push_back(leftItem);
						}
					}
				}

				return res;
			}

			std::vector<Object> ObjectDetector::getObjectsRects(SharedColorDiffsMatrix diffs)
			{
				const size_t width = diffs.get()->getWidth();
				const size_t height = diffs.get()->getHeight();
				const size_t columnWidth = width / mThreadsCount;
				mLogger->logInfo("ObjectDetector::getObjectsRects width:%zd height:%zd  threads:%d", width, height, mThreadsCount);
				std::shared_ptr<Matrix<uint16_t>> changes = ThresholdDiffChecker::getThresholdDiff(diffs, 
					mThreadsCount, mConfiguration.AffinityThreshold);
				std::vector<std::vector<Pixels>> res;

				std::vector<std::future<std::vector<Pixels>>> futures;

				for (size_t columnId = 0; columnId < mThreadsCount; ++columnId)
				{
					Pixels toCheck;
					Column column{ columnId*columnWidth, columnId*columnWidth + columnWidth };
					futures.push_back(std::async(startObjectsSearch, changes, mConfiguration.PixelStep, column));
				}

				for (auto &e : futures)
					res.push_back(e.get());

				mLogger->logInfo("ObjectDetector::getObjectsRects fin");
				return createObjectRects(res, mConfiguration.MinPixelsForObject);
			}

			std::vector<Object> ObjectDetector::getObjectsRects(std::shared_ptr<ImagePair> pair)
			{
				const size_t columnWidth = pair.get()->getWidth() / mThreadsCount;

				std::vector<std::vector<Pixels>> res;
				std::vector<std::future<std::vector<Pixels>>> futures;
				mLogger->logInfo("ObjectDetector::getObjectsRects pair threads:%d", mThreadsCount);

				for (size_t columnId = 0; columnId < mThreadsCount; ++columnId)
				{
					Pixels toCheck;
					Column column{ columnId*columnWidth, columnId*columnWidth + columnWidth };
					futures.push_back(std::async(startObjectsSearchInPair, pair, mConfiguration.PixelStep, column, mConfiguration));
				}

				for (auto &e : futures)
					res.push_back(e.get());

				mLogger->logInfo("ObjectDetector::getObjectsRects fin");
				return createObjectRects(res, mConfiguration.MinPixelsForObject);
			}
		}
	}
}
