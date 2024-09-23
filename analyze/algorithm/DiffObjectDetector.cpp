#include "DiffObjectDetector.h"
#include "analyze/ThresholdDiffChecker.h"
#include "analyze/AffinityComparer.h"
#include "analyze/algorithm/ImagePair.h"
#include <future>

namespace am::analyze::algorithm
{
	using ColorDiffsMatrix = common::types::Matrix<common::types::Color24bDiff>;
	using namespace am::common::types;
	using Pixels = std::vector<Pixel>;

	namespace
	{
		// optimized bfs depending to left/right borders for threads,
		// every thread will search in defined area(column) of image
		ObjectRectangle bfs(MatrixU16 &changes, Pixels &toCheck, ObjectRectangle &object, ImageRowSegment row)
		{
			Pixels nextCheck;

			for (auto &position : toCheck)
			{
				if (changes(position.rowId, position.colId) == common::CHANGE)
				{
					checkClosest(position.rowId, position.colId, nextCheck, object, row, changes.getWidth(), 1u);

					// if (isNew(object, position.rowId, position.colId))
					//{
					// object.emplace_back(position.rowId, position.colId);
					object.addPixel(position.rowId, position.colId);
					changes(position.rowId, position.colId) = 0;
					//}
				}
			}
			if (nextCheck.size())
				bfs(changes, nextCheck, object, row);

			return object;
		}

		static std::vector<ObjectRectangle> startObjectsSearch(MatrixU16 &changes, const Configuration& conf, const ImageRowSegment &row)
		{
			std::vector<ObjectRectangle> result;
			const size_t step = conf.PixelStep;
			// check all diffs on potential objects
			// if change found -> run bsf to figure out how many pixels included in this object
			// for (size_t rowId = step; rowId < changes.getHeight(); rowId += step)
			for (size_t rowId = row.start; rowId < row.end; rowId += step)
			{
				for (size_t colId = step; colId < changes.getWidth(); colId += step)
				{
					if (changes(rowId, colId) == common::CHANGE)
					{
						// std::vector<Pixel> obj;
						ObjectRectangle obj(rowId, colId);

						auto conns = checkConnections(rowId, colId, changes.getWidth(), row, 1u);
						result.emplace_back(bfs(changes, conns, obj, row));
					}
				}
			}
			return result;
		}
	}

	DiffObjectDetector::DiffObjectDetector(const size_t threads, const Configuration &conf, std::shared_ptr<am::common::Logger> &logger) : BfsObjectDetector(threads, conf, logger)
	{
	}

	DescObjects DiffObjectDetector::getObjectsRects(ColorDiffsMatrix &diffs)
	{
		const size_t width = diffs.getWidth();
		const size_t height = diffs.getHeight();
		const size_t rowWidth = height / mThreadsCount;
		std::vector<std::vector<ObjectRectangle>> res;
		std::vector<std::future<std::vector<ObjectRectangle>>> futures;

		mLogger->info("DiffObjectDetector::getObjectsRects width:%zd height:%zd  threads:%d", width, height, mThreadsCount);

		MatrixU16 changes = ThresholdDiffChecker::getThresholdDiff(diffs,
																   mThreadsCount, mThreadsCount);

		for (size_t rownId = 0; rownId < mThreadsCount; ++rownId)
		{
			Pixels toCheck;
			ImageRowSegment row_seg{rownId * rowWidth, rownId * rowWidth + rowWidth};
			futures.emplace_back(std::async(std::launch::async, startObjectsSearch, std::ref(changes), mConfiguration, row_seg));
		}

		for (auto &e : futures)
			res.push_back(e.get());

		mLogger->info("DiffObjectDetector::getObjectsRects fin");
		return createObjectRects(res, mConfiguration.MinPixelsForObject);
	}

	DescObjects DiffObjectDetector::getObjectsRects(ImagePair &pair)
	{
		ColorDiffsMatrix diffs = am::analyze::AffinityComparer::compare(pair.getFirst(), pair.getSecond(), mThreadsCount);
		return getObjectsRects(diffs);
	}
} // namespace am::analyze::algorithm
