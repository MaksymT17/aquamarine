#include "ImagePair.h"
#include "common/Context.hpp"
#include <common/AllocationException.hpp>
#include <cstdlib>

namespace recognition {
	namespace analyze {
		namespace algorithm {

			ImagePair::ImagePair(std::shared_ptr<common::Matrix<common::ColorChannels>> base, std::shared_ptr<common::Matrix<common::ColorChannels>> toCompare) :
				mBase(base),
				mCompare(toCompare)
			{
				if (mBase->getHeight() != mCompare->getHeight() || mBase->getWidth() != mCompare->getWidth())
				{
					std::string msg("ImagePair allocation failed, size of Images are different. Pair require equal size.");
					throw common::AllocationException(msg);
				}

				mWidth = mBase->getWidth();
				mHeight = mBase->getHeight();
			}

			size_t ImagePair::getWidth() const
			{
				return mWidth;
			}

			size_t ImagePair::getHeight() const
			{
				return mHeight;
			}

			uint16_t ImagePair::getAbsoluteDiff(const size_t rowId, const size_t colId) const
			{
				common::Matrix<common::ColorChannels>& baseRef = *mBase.get();
				common::Matrix<common::ColorChannels>& compareRef = *mCompare.get();

				return std::abs(baseRef(rowId, colId).r - compareRef(rowId, colId).r) +
					std::abs(baseRef(rowId, colId).g - compareRef(rowId, colId).g) +
					std::abs(baseRef(rowId, colId).b - compareRef(rowId, colId).b);
			}

		}
	}
}
