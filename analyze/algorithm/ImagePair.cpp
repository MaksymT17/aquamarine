#include "ImagePair.h"
#include "common/Context.hpp"
#include <common/exceptions/AmException.hpp>

namespace am
{
	namespace analyze
	{
		namespace algorithm
		{

			using namespace common::types;

			ImagePair::ImagePair(const Matrix<Color24b> &base, const Matrix<Color24b> &toCompare) : mBase(base),
																									mCompare(toCompare)
			{
				if (mBase.getHeight() != mCompare.getHeight() || mBase.getWidth() != mCompare.getWidth())
				{
					std::string msg("ImagePair allocation failed, size of Images are different. Pair require equal size.");
					throw common::exceptions::AmException(msg);
				}
			}

			size_t ImagePair::getWidth() const noexcept
			{
				return mBase.getWidth();
			}

			size_t ImagePair::getHeight() const noexcept
			{
				return mBase.getHeight();
			}

			uint16_t ImagePair::getAbsoluteDiff(const size_t rowId, const size_t colId) const noexcept
			{
				return std::abs(mBase(rowId, colId).r - mCompare(rowId, colId).r) +
					   std::abs(mBase(rowId, colId).g - mCompare(rowId, colId).g) +
					   std::abs(mBase(rowId, colId).b - mCompare(rowId, colId).b);
			}

			const common::types::Matrix<common::types::Color24b> &ImagePair::getFirst() const noexcept
			{
				return mBase;
			}

			const common::types::Matrix<common::types::Color24b> &ImagePair::getSecond() const noexcept
			{
				return mCompare;
			}
		}
	}
}
