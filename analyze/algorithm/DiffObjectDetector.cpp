#include "DiffObjectDetector.h"
#include "analyze/ThresholdDiffChecker.h"
#include "analyze/AffinityComparer.h"
#include "analyze/algorithm/ImagePair.h"
#include <future>

namespace am {
	namespace analyze {
		namespace algorithm {
			using SharedColorDiffsMatrix = std::shared_ptr<common::types::Matrix<common::types::Color24bDiff>>;
			using namespace am::common::types;
			using Pixels = std::vector<Pixel>;

			DiffObjectDetector::DiffObjectDetector(const size_t threads, std::shared_ptr<am::configuration::Configuration>& conf, std::shared_ptr<am::common::Logger>& logger) :
				BfsObjectDetector(threads, conf, logger)
			{
			}

			//optimized bfs depending to left/right borders for threads, 
			//every thread will search in defined area(column) of image
			Pixels bfs(Matrix<uint16_t>& changes, Pixels& toCheck, Pixels& object, Column col)
			{
				Pixels nextCheck;

				for (auto& position : toCheck)
				{
					if (changes(position.rowId, position.colId) == common::CHANGE)
					{
						Pixel newPos{ position.rowId, position.colId };

						checkClosest(newPos, nextCheck, object, col, changes.getHeight(), 1u);

						if (isNew(object, position.rowId, position.colId))
						{
							object.push_back(newPos);
							changes(position.rowId, position.colId) = 0;
						}
					}
				}
				if (nextCheck.size())
					bfs(changes, nextCheck, object, col);

				return object;
			}

			std::vector<Pixels> startObjectsSearch(std::shared_ptr<Matrix<uint16_t>> changes, size_t step, const Column& col)
			{
				auto& chRef = *changes.get();

				std::vector<Pixels> result;
				// check all diffs on potential objects
				//if change found -> run bsf to figure out how many pixels included in this object
				for (size_t rowId = step; rowId < chRef.getHeight(); rowId += step)
				{
					for (size_t colId = col.left; colId < col.right; colId += step)
					{
						if (chRef(rowId, colId) == common::CHANGE)
						{
							Pixel px{ rowId, colId };
							Pixels obj = { px };
							auto conns = checkConnections(px, chRef.getHeight(), col, 1u);
							result.push_back(bfs(chRef, conns, obj, col));
						}
					}
				}
				return result;
			}

			DescObjects DiffObjectDetector::getObjectsRects(SharedColorDiffsMatrix diffs)
			{
				const size_t width = diffs.get()->getWidth();
				const size_t height = diffs.get()->getHeight();
				const size_t columnWidth = width / mThreadsCount;
				std::vector<std::vector<Pixels>> res;
				std::vector<std::future<std::vector<Pixels>>> futures;

				mLogger->info("DiffObjectDetector::getObjectsRects width:%zd height:%zd  threads:%d", width, height, mThreadsCount);

				std::shared_ptr<Matrix<uint16_t>> changes = ThresholdDiffChecker::getThresholdDiff(diffs,
					mThreadsCount, mConfiguration->AffinityThreshold);

				for (size_t columnId = 0; columnId < mThreadsCount; ++columnId)
				{
					Pixels toCheck;
					Column column{ columnId*columnWidth, columnId*columnWidth + columnWidth };
					futures.push_back(std::async(std::launch::async, startObjectsSearch, changes, mConfiguration->PixelStep, column));
				}

				for (auto &e : futures)
					res.push_back(e.get());

				mLogger->info("DiffObjectDetector::getObjectsRects fin");
				return createObjectRects(res);
			}
			DescObjects DiffObjectDetector::getObjectsRects(std::shared_ptr<ImagePair> pair)
			{
				SharedColorDiffsMatrix diffs = am::analyze::AffinityComparer::compare(pair->getFirst(), pair->getSecond());
				return getObjectsRects(diffs);
			}


		}
	}
}