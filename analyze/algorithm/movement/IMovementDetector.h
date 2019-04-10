#pragma once

#include<memory>
#include<set>
#include"common/types/Matrix.hpp"
#include"common/types/ColorChannelsDiff.hpp"
#include "analyze/algorithm/Object.h"

namespace am {
	namespace analyze {
		namespace algorithm {

			class ImagePair;

			namespace movement {

				/// note: bitset for movements storage
				// bytes definition: 0, 1 - left, right
				// 2, 3 - top, bottom; 4 - disappear
				// all types can be mixed, except disappear
				struct MovementType
				{
					enum {
						LEFT = 0,
						RIGHT = 1,
						TOP = 2,
						BOTTOM = 3,
						DISAPPEAR = 4
					};
					std::bitset<5> mov;
				};

				// interface for Movements detection classes,
				// movements shall be provided depended to
				class IMovementDetector 
				{
				public:
					~IMovementDetector() = default;
					virtual std::vector<MovementType> getMovementsFromRects(std::multiset<Object, comparators::Descending>& first,
						std::multiset<Object, comparators::Descending>& second) = 0;
				};

			}
		}
	}
}
