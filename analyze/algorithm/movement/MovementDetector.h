#pragma once

#include<set>
#include<vector>
#include"analyze/algorithm/Object.h"
#include"analyze/algorithm/ImagePair.h"
#include"analyze/algorithm/ObjectDetector.h"
#include "common/Logger.hpp"
#include<bitset>
#include "IMovementDetector.h"

namespace am {
	namespace analyze {
		namespace algorithm {
			namespace movement {

				// method for collecting movements based on Object bounds, not on real pixel data
				static std::vector<MovementType> getMovementsFromRects(std::multiset<Object, 
					comparators::Descending>& base, std::multiset<Object, comparators::Descending>& toCheck, 
					std::shared_ptr<am::common::Logger>& logger) noexcept;

				class MovementDetector: public ObjectDetector, IMovementDetector {
				public:
					MovementDetector(const size_t threads, std::shared_ptr<am::configuration::Configuration>& conf, std::shared_ptr<am::common::Logger>& logger);
					~MovementDetector() = default;

					virtual std::vector<MovementType> getMovementsFromRects(std::multiset<Object, comparators::Descending>& first, 
						std::multiset<Object, comparators::Descending>& second) override;

					virtual DescObjects getObjectsRects(std::shared_ptr<ImagePair> pair) override;
				};
			}
		}
	}
}