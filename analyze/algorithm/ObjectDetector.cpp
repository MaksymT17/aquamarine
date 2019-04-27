#include "ObjectDetector.h"
#include "analyze/algorithm/ImagePair.h"
#include <future>

namespace am {
	namespace analyze {
		namespace algorithm {
			using namespace am::common::types;
			using Pixels = std::vector<Pixel>;

			ObjectDetector::ObjectDetector(const size_t threads,
				std::shared_ptr<am::configuration::Configuration>& conf,
				std::shared_ptr<am::common::Logger>& logger) :
				BfsObjectDetector(threads, conf, logger)
			{
			}

			// Time dependent execution, if max calculation time exceeded calculation should finilize calculation.
			Pixels bfs(ImagePair& pair, Matrix<uint16_t>& visited, Pixels& toCheck, Pixels& object,
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
						Pixel newPos{ position.rowId, position.colId };
						checkClosest(newPos, nextCheck, object, col, visited.getHeight(), conf.PixelStep);

						if (isNew(object, newPos))
						{
							object.push_back(newPos);
							visited(position.rowId, position.colId) = common::CHANGE;
						}
					}
				}
				if (nextCheck.size())
					bfs(pair, visited, nextCheck, object, col, startTime, conf);

				return object;
			}

			std::vector<Pixels> startObjectsSearchInPair(std::shared_ptr<ImagePair> pair, const Column& col,
				const configuration::Configuration& conf)
			{
				auto startTime = std::chrono::steady_clock::now();
				auto& pairRef = *pair.get();
				Matrix<uint16_t> changes(pairRef.getWidth(), pairRef.getHeight());

				std::vector<Pixels> resultList;
				// check all diffs on potential objects
				//if change found -> run bfs to figure out how many pixels included in this object
				for (size_t rowId = conf.PixelStep; rowId < pairRef.getHeight(); rowId += conf.PixelStep)
				{
					for (size_t colId = col.left; colId < col.right; colId += conf.PixelStep)
					{
						auto timeNow = std::chrono::steady_clock::now();
						std::chrono::duration<double> calcDuration = timeNow - startTime;
						if (calcDuration.count() >= conf.CalculationTimeLimit)
						{
							///todo: make Error notification about failed detection
							//mLogger->logInfo("Timelimit for calculation exceded, calculations TimeLimit:%f", conf.CalculationTimeLimit);
							return resultList;
						}
						else if (pairRef.getAbsoluteDiff(rowId, colId) > conf.AffinityThreshold &&
							changes(rowId, colId) != common::CHANGE)
						{
							Pixel px{ rowId, colId };
							Pixels obj{ px };
							auto conns = checkConnections(px, pairRef.getHeight(), col, conf.PixelStep);
							resultList.push_back(bfs(pairRef, changes, conns, obj, col, startTime, conf));
						}
					}
				}
				return resultList;
			}
			DescObjects ObjectDetector::getObjectsRects(std::shared_ptr<ImagePair> pair)
			{
				const size_t columnWidth = pair.get()->getWidth() / mThreadsCount;

				std::vector<std::vector<Pixels>> res;
				std::vector<std::future<std::vector<Pixels>>> futures;
				mLogger->info("ObjectDetector::getObjectsRects pair threads:%d", mThreadsCount);

				for (size_t columnId = 0; columnId < mThreadsCount; ++columnId)
				{
					Column column{ columnId*columnWidth, columnId*columnWidth + columnWidth };
					futures.push_back(std::async(std::launch::async, startObjectsSearchInPair, pair, column, *mConfiguration));
				}

				for (auto &e : futures)
					res.push_back(e.get());

				return createObjectRects(res);
			}
		}
	}
}