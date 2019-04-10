#pragma once
#include "BfsObjectDetector.h"

namespace am {
	namespace analyze {
		namespace algorithm {

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