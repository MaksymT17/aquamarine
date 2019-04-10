#pragma once
#include "IObjectDetector.h"
#include "configuration/Configuration.hpp"
#include "common/Logger.hpp"

namespace am {

	namespace analyze {
		namespace algorithm {

			class ImagePair;

			bool isNew(std::vector<Pixel>& object, Pixel newPos);
			void pushCheckIfNew(std::vector<Pixel>& object, std::vector<Pixel>& toCheck, Pixel newPos);
			void checkClosest(Pixel& pos, std::vector<Pixel>& nextCheck, std::vector<Pixel>& object, Column col, const size_t& height);
			std::vector<Pixel> checkConnections(const Pixel& px, const size_t& height, const Column& col);

			// abstract class for object detection, extracted data should be reusable for next iterations
			// return value of detection is vector of Objects - Rectangled area
			class BfsObjectDetector : public IObjectDetector
			{
			public:
				virtual ~BfsObjectDetector() = default;

				//all calculation with diffs, no time limits
				//DescObjects getObjectsRects(std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>> diffs);

				//search of ebjects within ImagePair data provided
				virtual DescObjects getObjectsRects(std::shared_ptr<ImagePair> pair) = 0;

				// creation of ordered objects list based
				DescObjects createObjectRects(std::vector<std::vector<std::vector<Pixel>>>& objPixels);

			protected:
				// abstract class construction limited, aggregate common functions and interface requirement 
				BfsObjectDetector(const size_t threads, std::shared_ptr<am::configuration::Configuration>& conf, std::shared_ptr<am::common::Logger>& logger);

				size_t mThreadsCount;
				std::shared_ptr<am::configuration::Configuration> mConfiguration;
				std::shared_ptr<am::common::Logger> mLogger;
			};
		}
	}
}