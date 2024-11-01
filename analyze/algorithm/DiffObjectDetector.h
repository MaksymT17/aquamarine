#pragma once
#include "BfsObjectDetector.h"

namespace am::analyze::algorithm
{

	using ColorDiffsMatrix = common::types::Matrix<common::types::Color24bDiff>;

	class DiffObjectDetector : public BfsObjectDetector
	{
	public:
		DiffObjectDetector(const size_t threads,
						   const Configuration &conf,
						   std::shared_ptr<am::common::Logger> &logger);
		~DiffObjectDetector() = default;

		virtual DescObjects getObjectsRects(ImagePair &pair) override;

		// all calculation with diffs, no time limits,
		// warning: in case of big images and not optimal configuration calculation can take long time
		DescObjects getObjectsRects(ColorDiffsMatrix &diffs);
	};

} // namespace am::analyze::algorithm
