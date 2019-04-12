#include "MovementDetector.h"
#include "common/exceptions/AmException.hpp"
#include "common/types/Matrix.hpp"

namespace am {
	namespace analyze {
		namespace algorithm {
			namespace movement {

				using namespace am::common::types;
				
				static MovementType getMovementFromObjRects(const Object& base, const Object& toCheck,
					std::shared_ptr<am::common::Logger>& logger) noexcept
				{
					MovementType type;
					if (!(base.getMinHeight() > toCheck.getMaxHeight() || base.getMinWidth() > toCheck.getMaxWidth()))
					{
						if (base.getMinWidth() > toCheck.getMinWidth())
							type.mov[MovementType::LEFT] = true;
						if (base.getMaxWidth() < toCheck.getMaxWidth())
							type.mov[MovementType::RIGHT] = true;
						if (base.getMinHeight() > toCheck.getMinHeight())
							type.mov[MovementType::BOTTOM] = true;
						if (base.getMaxHeight() < toCheck.getMaxHeight())
							type.mov[MovementType::TOP] = true;
					}
					else
					{
						type.mov[MovementType::MOVELESS] = true;
					}

					return type;
				}

				MovementDetector::MovementDetector(const size_t threads, std::shared_ptr<am::configuration::Configuration>& conf, std::shared_ptr<am::common::Logger>& logger) :
					ObjectDetector(threads, conf, logger)
				{
				}

				void MovementDetector::setTrackingObjects(DescObjects& objs)
				{
					if (!objs.size())
						mLogger->warn("%s, empty list of objects.", __func__);

					mObjects = objs;
				}

				/// every single object can devided to many or be merged to bigger one,
				/// func: should catch every case and be able to use learning principles as AI
				/// 'found' out parameter of ojects(in case of multiple objects found) related to single input object
				MovementType MovementDetector::getMovementForObject(const Object& obj, ImagePairPtr& pair, Objects& found)
				{
					Matrix<uint16_t> changes(pair->getWidth(), pair->getHeight());
					auto startTime = std::chrono::steady_clock::now();

					for (const auto& px : obj.getPixels())
					{
						if (pair->getAbsoluteDiff(px.rowId, px.colId) > mConfiguration->AffinityThreshold &&
							changes(px.rowId, px.colId) != common::CHANGE)
						{
							Pixels pxs{ px };
							auto conns = checkConnections(px, pair->getHeight(), { 0u, pair->getWidth() });
							auto objFound = bfs(*pair, changes, conns, pxs, { 0u, pair->getWidth() }, startTime, *mConfiguration);
							found.push_back(objFound);
						}
					}

					MovementType movement;
					if (!found.size())
					{
						movement.mov[MovementType::MOVELESS] = true;
						return movement;
					}
					else if (found.size() == 1) {
						//object found
						return getMovementFromObjRects(obj, *found.begin(), mLogger);
					}

					/// fullfill all movements from collected vector of objects
					for (const auto& newObj : found)
					{
						MovementType current = getMovementFromObjRects(obj, *found.begin(), mLogger);
						for (int pos = 0; pos < current.mov.size(); pos++)
						{
							if (current.mov[pos])
								movement.mov.set(pos);
						}
					}
					return movement;
				}

				Movements MovementDetector::analyze(ImagePairPtr& pair, DescObjects& newObjects)
				{
					if (!mObjects.size())
					{
						std::string msg("MovementDetector::getMovs Empty list of objects!");
						throw am::common::exceptions::AmException(msg);
					}
					Movements movs;
					Objects newlyFoundObjs;

					size_t id = 0;
					for (const auto& obj : mObjects)
					{
						movs[id++] = getMovementForObject(obj, pair, newlyFoundObjs);
					}

					// make sense to set mObjects for next iteration
					// shall be configurable, user can set objects on his own decision

					newObjects.insert(newlyFoundObjs.begin(), newlyFoundObjs.end());

					return movs;
				}

			}
		}
	}
}
