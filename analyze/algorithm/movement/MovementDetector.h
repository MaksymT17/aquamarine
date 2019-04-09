#pragma once

#include<set>
#include<vector>
#include"analyze/algorithm/Object.h"
#include"analyze/algorithm/ImagePair.h"
#include "common/Logger.hpp"
#include<bitset>

namespace am {
	namespace analyze {
		namespace algorithm {
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

				// method for collecting movements based on Object bounds, not on real pixel data
				static std::vector<MovementType> getMovementsFromRects(std::multiset<Object, comparators::Descending>& base, std::multiset<Object, comparators::Descending>& toCheck, std::shared_ptr<am::common::Logger>& logger) noexcept;

				class MovementDetector {
					///todo: requires 2 implementations: based on rects(fast), based on weight(slow, precize)
				};
			}
		}
	}
}