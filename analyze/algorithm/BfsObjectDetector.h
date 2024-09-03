#pragma once
#include "IObjectDetector.h"
#include "sh_mem/Message.hpp"
#include "common/Logger.hpp"
#include "Object.h"

namespace am::analyze::algorithm
{

	class ImagePair;

	bool isNew(const std::vector<Pixel> &object, size_t rowId, size_t colId) noexcept;
	void pushCheckIfNew(const std::vector<Pixel> &object, std::vector<Pixel> &toCheck, size_t rowId, size_t colId);
	void checkClosest(size_t rowId, size_t colId, std::vector<Pixel> &nextCheck, ObjectRectangle &object, ImageRowSegment &row, const size_t width, const size_t step) noexcept;
	std::vector<Pixel> checkConnections(size_t rowId, size_t colId, const size_t &width, const ImageRowSegment &col, const size_t step) noexcept;

	// abstract class for object detection, extracted data should be reusable for next iterations
	// return value of detection is vector of Objects - Rectangled area
	class BfsObjectDetector : public IObjectDetector
	{
	public:
		~BfsObjectDetector() = default;

		// all calculation with diffs, no time limits
		// DescObjects getObjectsRects(std::shared_ptr<common::types::Matrix<common::types::Color24bDiff>> diffs);

		// search of ebjects within ImagePair data provided
		virtual DescObjects getObjectsRects(ImagePair &pair) override = 0;

		// creation of ordered objects list: Descending(bigger first)
		DescObjects createObjectRects(std::vector<std::vector<ObjectRectangle>> &objPixels,
									  const size_t minPixels);

	protected:
		// abstract class construction limited, aggregate common functions and interface requirement
		BfsObjectDetector(const size_t threads, const Configuration &conf,
						  std::shared_ptr<am::common::Logger> &logger);

		size_t mThreadsCount;
		Configuration mConfiguration;
		std::shared_ptr<am::common::Logger> mLogger;
	};
} // namespace am::analyze::algorithm
