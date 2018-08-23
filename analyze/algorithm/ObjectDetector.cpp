#include "ObjectDetector.h"
#include<future>
#include<vector>
#include<algorithm>
#include"analyze/ThresholdDiffChecker.h"

using namespace common;

namespace recognition {
	namespace analyze {
		namespace algorithm {

			ObjectDetector::ObjectDetector(const size_t threadsCount) :
				mThreadsCount(threadsCount),
				mPixelStep(1u),
				mThreshold(25u)
			{
			}
			
			bool isNew(std::vector<Position>& object, Position newPos)
			{
				for (auto pos : object)
					if (pos.colId == newPos.colId && pos.rowId == newPos.rowId)
						return false;

				return true;
			}

			void pushCheckIfNew(std::vector<Position>& object, std::vector<Position>& toCheck, Position newPos)
			{
				if (isNew(object, newPos))
					toCheck.push_back(newPos);
			}

			std::vector<Position> dfs(Matrix<uint16_t>& changes, std::vector<Position>& toCheck, std::vector<Position>& object, size_t counter)
			{
				std::vector<Position> nextCheck;

				for (auto& position : toCheck)
				{
					if ((position.rowId >= 0 && position.colId >= 0 && position.rowId < changes.getHeight() && position.colId < changes.getWidth())
						&& changes(position.rowId, position.colId) == CHANGE)
					{
						//check all neighbours when CHANGE visible, push new to CHeck in next iteration

						pushCheckIfNew(object, nextCheck, Position{ position.rowId - 1, position.colId });
						pushCheckIfNew(object, nextCheck, Position{ position.rowId + 1, position.colId });
						pushCheckIfNew(object, nextCheck, Position{ position.rowId , position.colId - 1 });
						pushCheckIfNew(object, nextCheck, Position{ position.rowId , position.colId + 1 });

						Position newPos{ position.rowId, position.colId };

						if (isNew(object, newPos))
						{
							object.push_back(newPos);
							changes(position.rowId, position.colId) = counter;
						}
					}
				}
				if (nextCheck.size())
					dfs(changes, nextCheck, object, counter);

				return object;
			}

			std::vector<std::vector<Position>> startObjectsSearch(std::shared_ptr<Matrix<uint16_t>> changes, size_t step, size_t counter)
			{
				auto& chRef = *changes.get();

				std::vector<std::vector<Position>> resultList;
				// check all diffs on potential objects
				//if change found -> run DFS to figure out how many pixels included in this object
				for (size_t rowId = step; rowId < changes.get()->getHeight(); rowId += step)
				{
					for (size_t colId = step; colId < changes.get()->getWidth(); colId += step)
					{
						if (chRef(rowId, colId) == CHANGE)
						{
							std::vector<Position> obj = { Position{rowId, colId} };
							std::vector<Position> toCheck = { Position{ rowId - 1, colId },Position{ rowId + 1, colId },
								Position{ rowId, colId - 1 },Position{ rowId, colId + 1 } };

							resultList.push_back(dfs(chRef, toCheck, obj, counter++));
						}
					}
				}
				return resultList;
			}

			std::vector<std::vector<Position>> ObjectDetector::getObjectsCount(std::shared_ptr<Matrix<ColorChannelsDiff>> diffs)
			{
				const size_t width = diffs.get()->getWidth();
				const size_t height = diffs.get()->getHeight();
				size_t count = 0;
				const size_t columnWidth = width / mThreadsCount;

				std::shared_ptr<Matrix<uint16_t>> changes = ThresholdDiffChecker::getThresholdDiff(diffs, mThreadsCount, mThreshold);
				std::vector<Position> toCheck;
				std::vector<std::vector<Position>> res = startObjectsSearch(changes, mPixelStep, count);
				/*
				std::vector<std::future<size_t>> futures;

				for (size_t columnId = 0; columnId < mThreadsCount; ++columnId)
				{
					std::vector<Position> toCheck;
					futures.push_back(std::async(doDfsSearch, toCheck,changes, count));
				}

				for (auto &e : futures)
					e.get();
					*/
				return res;
			}

		}
	}
}