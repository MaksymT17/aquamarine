#include "MovementDetector.h"
#include "common/exceptions/AmException.hpp"
#include "common/types/Matrix.hpp"

namespace am::analyze::algorithm::movement
{

	using namespace am::common::types;

	MovementType MovementDetector::getMovementFromObjRects(const ObjectRectangle &base,
														   const ObjectRectangle &toCheck) noexcept
	{
		MovementType type;
		if (!(base.getMinHeight() > toCheck.getMaxHeight() || base.getLeft() > toCheck.getRight()))
		{
			if (base.getLeft() > toCheck.getLeft())
				type.mov[MovementType::LEFT] = true;
			if (base.getRight() < toCheck.getRight())
				type.mov[MovementType::RIGHT] = true;
			if (base.getMinHeight() < toCheck.getMinHeight())
				type.mov[MovementType::BOTTOM] = true;
			if (base.getMaxHeight() > toCheck.getMaxHeight())
				type.mov[MovementType::TOP] = true;
			if (base.getLeft() == toCheck.getLeft() && base.getRight() == toCheck.getRight() &&
				base.getMinHeight() == toCheck.getMinHeight() && base.getMaxHeight() == toCheck.getMaxHeight())
				type.mov[MovementType::STEALTH] = true;
		}

		return type;
	}

	MovementDetector::MovementDetector(const size_t threads,
									   const Configuration &conf,
									   std::shared_ptr<am::common::Logger> &logger)
		: ObjectDetector(threads, conf, logger)
	{
	}

	void MovementDetector::setTrackingObjects(DescObjects &objs)
	{
		if (!objs.size())
			mLogger->warn("%s, empty list of objects.", __func__);

		mObjects = objs;
	}

	/// every single object can be devided to many or be merged to the bigger one,
	/// func: should catch every case and be able to use learning principles as AI
	/// 'found' out parameter of ojects(in case of multiple objects found) related to single input object
	MovementType MovementDetector::getMovementForObject(const ObjectRectangle &obj,
														ImagePairPtr &pair,
														std::vector<ObjectRectangle> &found)
	{
		MatrixU16 changes(pair->getWidth(), pair->getHeight());
		auto startTime = std::chrono::steady_clock::now();

		for (size_t rowId = obj.getMinHeight(); rowId < obj.getMaxHeight();
			 rowId += mConfiguration.PixelStep)
		{
			for (size_t colId = obj.getLeft(); colId < obj.getRight(); colId += mConfiguration.PixelStep)
			{
				if (pair->getAbsoluteDiff(rowId, colId) > mConfiguration.AffinityThreshold &&
					changes(rowId, colId) != common::CHANGE)
				{
					ObjectRectangle pxs(rowId, colId);
					auto conns = checkConnections(rowId, colId, pair->getHeight(), {0u, pair->getWidth()}, mConfiguration.PixelStep);
					auto objFound = bfs(*pair, changes, conns, pxs, {0u, pair->getWidth()}, startTime, mConfiguration);
					found.emplace_back(objFound);
				}
			}
		}

		MovementType movement;
		if (!found.size())
		{
			movement.mov[MovementType::STEALTH] = true;
			return movement;
		}
		else if (found.size() == 1)
		{
			// object found
			return getMovementFromObjRects(obj, *found.begin());
		}

		/// fullfill all movements from collected vector of objects
		for (const auto &newObj : found)
		{
			MovementType current = getMovementFromObjRects(obj, *found.begin());
			for (int pos = 0; pos < current.mov.size(); pos++)
			{
				if (current.mov[pos])
					movement.mov.set(pos);
			}
		}
		return movement;
	}

	Movements MovementDetector::analyze(ImagePairPtr &pair, DescObjects &newObjects)
	{
		if (!mObjects.size())
		{
			std::string msg("MovementDetector::getMovs Empty list of objects!");
			throw am::common::exceptions::AmException(msg);
		}
		Movements movs;
		std::vector<ObjectRectangle> newlyFoundObjs;

		for (const auto &obj : mObjects)
		{
			movs.emplace_back(getMovementForObject(obj, pair, newlyFoundObjs));
		}

		// make sense to set mObjects for next iteration
		// shall be configurable, user can set objects on his own decision

		newObjects.insert(newlyFoundObjs.begin(), newlyFoundObjs.end());

		return movs;
	}
} // namespace am::analyze::algorithm::movement
