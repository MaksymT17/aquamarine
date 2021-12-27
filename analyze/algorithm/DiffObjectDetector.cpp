#include "DiffObjectDetector.h"
#include "analyze/ThresholdDiffChecker.h"
#include "analyze/AffinityComparer.h"
#include "analyze/algorithm/ImagePair.h"
#include <future>

namespace am
{
	namespace analyze
	{
		namespace algorithm
		{
			using ColorDiffsMatrix = common::types::Matrix<common::types::Color24bDiff>;
			using namespace am::common::types;
			using Pixels = std::vector<Pixel>;

			DiffObjectDetector::DiffObjectDetector(const size_t threads, std::shared_ptr<am::configuration::Configuration> &conf, std::shared_ptr<am::common::Logger> &logger) : BfsObjectDetector(threads, conf, logger)
			{
			}

			// optimized bfs depending to left/right borders for threads,
			// every thread will search in defined area(column) of image
			Pixels bfs(MatrixU16 &changes, Pixels &toCheck, Pixels &object, Column col)
			{
				Pixels nextCheck;

				for (auto &position : toCheck)
				{
					if (changes(position.rowId, position.colId) == common::CHANGE)
					{
						checkClosest(position.rowId, position.colId, nextCheck, object, col, changes.getHeight(), 1u);

						if (isNew(object, position.rowId, position.colId))
						{
							object.emplace_back(position.rowId, position.colId);
							changes(position.rowId, position.colId) = 0;
						}
					}
				}
				if (nextCheck.size())
					bfs(changes, nextCheck, object, col);

				return object;
			}

			std::vector<Pixels> startObjectsSearch(MatrixU16 &changes, am::configuration::Configuration *conf, const Column &col)
			{
				std::vector<Pixels> result;
				const size_t step = conf->PixelStep;
				// check all diffs on potential objects
				// if change found -> run bsf to figure out how many pixels included in this object
				for (size_t rowId = step; rowId < changes.getHeight(); rowId += step)
				{
					for (size_t colId = col.left; colId < col.right; colId += step)
					{
						if (changes(rowId, colId) == common::CHANGE)
						{
							std::vector<Pixel> obj;

							auto conns = checkConnections(rowId, colId, changes.getHeight(), col, 1u);
							result.emplace_back(bfs(changes, conns, obj, col));
						}
					}
				}
				return result;
			}

			DescObjects DiffObjectDetector::getObjectsRects(ColorDiffsMatrix &diffs)
			{
				const size_t width = diffs.getWidth();
				const size_t height = diffs.getHeight();
				const size_t columnWidth = width / mThreadsCount;
				std::vector<std::vector<Pixels>> res;
				std::vector<std::future<std::vector<Pixels>>> futures;

				mLogger->info("DiffObjectDetector::getObjectsRects width:%zd height:%zd  threads:%d", width, height, mThreadsCount);

				MatrixU16 changes = ThresholdDiffChecker::getThresholdDiff(diffs,
																		   mThreadsCount, mThreadsCount);

				for (size_t columnId = 0; columnId < mThreadsCount; ++columnId)
				{
					Pixels toCheck;
					Column column{columnId * columnWidth, columnId * columnWidth + columnWidth};
					futures.emplace_back(std::async(std::launch::async, startObjectsSearch, std::ref(changes), mConfiguration.get(), column));
				}

				for (auto &e : futures)
					res.push_back(e.get());

				mLogger->info("DiffObjectDetector::getObjectsRects fin");
				return createObjectRects(res, mConfiguration->MinPixelsForObject);
			}

			DescObjects DiffObjectDetector::getObjectsRects(ImagePair &pair)
			{
				ColorDiffsMatrix diffs = am::analyze::AffinityComparer::compare(pair.getFirst(), pair.getSecond(), mThreadsCount);
				return getObjectsRects(diffs);
			}
		}
	}
}