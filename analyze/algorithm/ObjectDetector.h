#pragma once

#include "BfsObjectDetector.h"
#include "sh_mem/Message.hpp"

namespace am::analyze::algorithm
{

	ObjectRectangle bfs(const ImagePair &pair, common::types::MatrixU16 &visited,
						std::vector<Pixel> &toCheck, ObjectRectangle &object,
						ImageRowSegment row, std::chrono::steady_clock::time_point &startTime,
						const Configuration &conf);

	class ObjectDetector : public BfsObjectDetector
	{
	public:
		ObjectDetector(const size_t threads, const Configuration &conf,
					   std::shared_ptr<am::common::Logger> &logger);
		~ObjectDetector() = default;

		// time for every collecting results defined in configuration
		// can be adjusted, but some objects can be not found because of terminated calculations
		virtual DescObjects getObjectsRects(ImagePair &pair) override;
	};

} // namespace am::analyze::algorithm
