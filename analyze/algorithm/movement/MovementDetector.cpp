#include "MovementDetector.h"

namespace am {
	namespace analyze {
		namespace algorithm {
			namespace movement {

				static std::vector<MovementType> getMovementsFromRects(std::multiset<Object, comparators::Descending>& base, 
					std::multiset<Object, comparators::Descending>& toCheck, std::shared_ptr<am::common::Logger>& logger) noexcept
				{
					std::vector<MovementType> result;
					for (const auto& check : toCheck)
					{
						MovementType type;
						bool isFound;
						for (const auto& b : base)
						{
							isFound = !(b.getMinHeight() > check.getMaxHeight() || b.getMinWidth() > check.getMaxWidth());
							if (isFound)
							{
								if (b.getMinWidth() > check.getMinWidth())
									type.mov[MovementType::LEFT] = true;
								if (b.getMaxWidth() < check.getMaxWidth())
									type.mov[MovementType::RIGHT] = true;
								if (b.getMinHeight() > check.getMinHeight())
									type.mov[MovementType::BOTTOM] = true;
								if (b.getMaxHeight() < check.getMaxHeight())
									type.mov[MovementType::TOP] = true;

								break;
							}
						}
						if (!isFound)
							type.mov[MovementType::DISAPPEAR] = true;

						result.push_back(type);
					}

					return result;
				}

				MovementDetector::MovementDetector(const size_t threads, std::shared_ptr<am::configuration::Configuration>& conf, std::shared_ptr<am::common::Logger>& logger) :
					ObjectDetector(threads, conf, logger)
				{
				}

				///todo: implement based on stencil search, where only object rects should be checked
				std::vector<MovementType> MovementDetector::getMovementsFromRects(std::multiset<Object, comparators::Descending>& first, std::multiset<Object, comparators::Descending>& second)
				{
					return std::vector<MovementType>();
				}

				DescObjects MovementDetector::getObjectsRects(std::shared_ptr<ImagePair> pair)
				{
					return DescObjects();
				}

			}
		}
	}
}
