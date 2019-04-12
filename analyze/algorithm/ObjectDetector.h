#pragma once

#include "BfsObjectDetector.h"

namespace am {

	namespace analyze {
		namespace algorithm {

			std::vector<Pixel> bsf(ImagePair& pair, common::types::Matrix<uint16_t>& visited, std::vector<Pixel>& toCheck, std::vector<Pixel>& object,
				Column col, std::chrono::steady_clock::time_point& startTime, const configuration::Configuration& conf);

			class ObjectDetector : public BfsObjectDetector {
			public:
				ObjectDetector(const size_t threads, std::shared_ptr<am::configuration::Configuration>& conf, std::shared_ptr<am::common::Logger>& logger);
				~ObjectDetector() = default;

				// time for every collecting results defined in configuration
				// can be adjusted, but some objects can be not found because of terminated calculations
				virtual DescObjects getObjectsRects(std::shared_ptr<ImagePair> pair) override;
			};

		}
	}
}