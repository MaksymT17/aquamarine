#pragma once

#include <memory>
#include <set>
#include "analyze/algorithm/Object.h"

namespace am
{
	namespace analyze
	{
		namespace algorithm
		{

			class ImagePair;

			namespace movement
			{

				/// note: bitset for movements storage
				// bytes definition: 0, 1 - left, right
				// 2, 3 - top, bottom; 4 - disappear
				// all types can be mixed, except stealth
				// bitmask with enabled bits:0,1,2,3 means expand of object
				struct MovementType
				{
					enum
					{
						LEFT = 0,
						RIGHT = 1,
						TOP = 2,
						BOTTOM = 3,
						STEALTH = 4
					};
					std::bitset<5> mov;
				};

				using Objects = std::vector<Object>;
				using DescObjects = std::multiset<ObjectRectangle, comparators::Descending>;
				using ImagePairPtr = std::shared_ptr<ImagePair>;
				using Movements = std::vector<MovementType>;

				// interface for Movements detection classes,
				// movements shall be realted to Tracking objects
				class IMovementDetector
				{
				public:
					~IMovementDetector() = default;

					virtual void setTrackingObjects(DescObjects &objs) = 0;
					virtual MovementType getMovementForObject(const ObjectRectangle &obj, ImagePairPtr &pair, std::vector<ObjectRectangle> &found) = 0;
					virtual Movements analyze(ImagePairPtr &pair, DescObjects &newObjects) = 0;
				};

			}
		}
	}
}
