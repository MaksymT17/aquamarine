#include "ImagePair.h"
#include "common/Context.hpp"
#include <common/exceptions/AmException.hpp>

namespace am {
	namespace analyze {
		namespace algorithm {

			using namespace common::types;

			ImagePair::ImagePair(std::shared_ptr<Matrix<Color24b>> base, std::shared_ptr<Matrix<Color24b>> toCompare) :
				mBase(base),
				mCompare(toCompare)
			{
				if (mBase->getHeight() != mCompare->getHeight() || mBase->getWidth() != mCompare->getWidth())
				{
					std::string msg("ImagePair allocation failed, size of Images are different. Pair require equal size.");
					throw common::exceptions::AmException(msg);
				}

			}

			size_t ImagePair::getWidth() const noexcept
			{
				return mBase->getWidth();
			}

			size_t ImagePair::getHeight() const noexcept
			{
				return mBase->getHeight();
			}

			uint16_t ImagePair::getAbsoluteDiff(const size_t rowId, const size_t colId) const noexcept
			{
				Matrix<Color24b>& baseRef = *mBase.get();
				Matrix<Color24b>& compareRef = *mCompare.get();

				return std::abs(baseRef(rowId, colId).r - compareRef(rowId, colId).r) +
					std::abs(baseRef(rowId, colId).g - compareRef(rowId, colId).g) +
					std::abs(baseRef(rowId, colId).b - compareRef(rowId, colId).b);
			}

			std::shared_ptr<common::types::Matrix<common::types::Color24b>> ImagePair::getFirst() const noexcept
			{
				return mBase;
			}

			std::shared_ptr<common::types::Matrix<common::types::Color24b>> ImagePair::getSecond() const noexcept
			{
				return mCompare;
			}
		}
	}
}
