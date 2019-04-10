#pragma once
#include "BfsObjectDetector.h"

namespace am {
	namespace analyze {
		namespace algorithm {

			class DiffObjectDetector : public BfsObjectDetector {
				using SharedColorDiffsMatrix = std::shared_ptr<common::types::Matrix<common::types::ColorChannelsDiff>>;

			public:
				DiffObjectDetector(const size_t threads, std::shared_ptr<am::configuration::Configuration>& conf, std::shared_ptr<am::common::Logger>& logger);
				~DiffObjectDetector();

				virtual DescObjects getObjectsRects(std::shared_ptr<ImagePair> pair) override;
				
				//all calculation with diffs, no time limits,
				//warning: in case of big images and not optimal configuration calculation can take long time
				DescObjects getObjectsRects(SharedColorDiffsMatrix diffs);
			};

		}
	}
}