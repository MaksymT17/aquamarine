#pragma once

#include "BfsObjectDetector.h"

namespace am
{

	namespace analyze
	{
		namespace algorithm
		{

			ObjectRectangle bfs(const ImagePair &pair, common::types::MatrixU16 &visited,
								std::vector<Pixel> &toCheck, ObjectRectangle &object,
								ImageRowSegment row, std::chrono::steady_clock::time_point &startTime,
								const configuration::Configuration &conf);

			class ObjectDetector : public BfsObjectDetector
			{
			public:
				ObjectDetector(const size_t threads, std::shared_ptr<am::configuration::Configuration> &conf,
							   std::shared_ptr<am::common::Logger> &logger);
				~ObjectDetector() = default;

				// time for every collecting results defined in configuration
				// can be adjusted, but some objects can be not found because of terminated calculations
				virtual DescObjects getObjectsRects(ImagePair &pair) override;
			};

		}
	}
}