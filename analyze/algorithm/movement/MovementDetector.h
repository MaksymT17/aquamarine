#pragma once

#include<set>
#include<vector>
#include"analyze/algorithm/Object.h"
#include"analyze/algorithm/ImagePair.h"
#include"analyze/algorithm/ObjectDetector.h"
#include "common/Logger.hpp"

#include "IMovementDetector.h"

namespace am {
	namespace analyze {
		namespace algorithm {
			namespace movement {

				using Pixels = std::vector<Pixel>;

				// implementation for IMovementDetector, which will check only areas of interested, previously found objects
				// Area out of provided Object list, shall be ignored, except only if object from list moved out of his own area or extended
				class MovementDetector : public ObjectDetector, IMovementDetector
				{
				public:
					MovementDetector(const size_t threads, std::shared_ptr<am::configuration::Configuration>& conf, std::shared_ptr<am::common::Logger>& logger);
					~MovementDetector() = default;

					// set objects which will be used for BFS search, like stencil on whole Image
					// without objects check movement has no sense
					virtual void setTrackingObjects(DescObjects& objs) override;

					// get Movement details for particular Object
					// will be checked area of this object based on Image pair provided
					// out of this area BFS will be not started
					virtual MovementType getMovementForObject(const Object& obj, ImagePairPtr& pair, Objects& found) override;

					// get movement for every objects provided by setTrackingObjects
					// ref of newObjects will be fulfilled with newly added objects
					virtual Movements analyze(ImagePairPtr& pair, DescObjects& newObjects) override;

				private:
					std::multiset<Object, comparators::Descending> mObjects;

				};
			}
		}
	}
}