#include "ImagePair.h"
#include "common/Context.hpp"
#include <common/exceptions/AllocationException.hpp>

namespace am {
	namespace analyze {
		namespace algorithm {

			using namespace common::types;

			ImagePair::ImagePair(std::shared_ptr<Matrix<ColorChannels>> base, std::shared_ptr<Matrix<ColorChannels>> toCompare) :
				mBase(base),
				mCompare(toCompare)
			{
				if (mBase->getHeight() != mCompare->getHeight() || mBase->getWidth() != mCompare->getWidth())
				{
					std::string msg("ImagePair allocation failed, size of Images are different. Pair require equal size.");
					throw common::exceptions::AllocationException(msg);
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
				Matrix<ColorChannels>& baseRef = *mBase.get();
				Matrix<ColorChannels>& compareRef = *mCompare.get();

				return std::abs(baseRef(rowId, colId).r - compareRef(rowId, colId).r) +
					std::abs(baseRef(rowId, colId).g - compareRef(rowId, colId).g) +
					std::abs(baseRef(rowId, colId).b - compareRef(rowId, colId).b);
			}
			std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> ImagePair::getFirst() const
			{
				return mBase;
			}
			std::shared_ptr<common::types::Matrix<common::types::ColorChannels>> ImagePair::getSecond() const
			{
				return mCompare;
			}
		}
	}
}
