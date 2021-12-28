#include "Object.h"
#include <stdio.h>

namespace am
{
	namespace analyze
	{
		namespace algorithm
		{

			Object::Object(std::vector<Pixel> &pixels)
				: ObjectBase::ObjectBase(pixels.begin()->rowId, pixels.begin()->colId), mPixels(pixels)
			{
				for (const auto &pixel : pixels)
				{
					if (mLeft > pixel.colId)
						mLeft = pixel.colId;
					else if (mRight < pixel.colId)
						mRight = pixel.colId;

					if (mMin_height > pixel.rowId)
						mMin_height = pixel.rowId;
					else if (mMax_height < pixel.rowId)
						mMax_height = pixel.rowId;
				}
			}

			bool ObjectBase::isMeargableToLeft(ObjectBase &leftObj) const noexcept
			{
				if (leftObj.getRight() + 1 == mLeft)
				{
					if (getMinHeight() <= leftObj.getMinHeight() && leftObj.getMinHeight() <= getMaxHeight() ||
						getMinHeight() <= leftObj.getMaxHeight() && leftObj.getMaxHeight() <= getMaxHeight())
						return true;
				}
				return false;
			}

			void ObjectBase::mergeToMe(ObjectBase &toCompare) noexcept
			{
				mPixelsCount += toCompare.mPixelsCount;

				// note: next line can be time costly, but important in Movement detection
				// mPixels.insert(mPixels.end(), toCompare.getPixels().begin(), toCompare.getPixels().end());

				if (mLeft > toCompare.mLeft)
					mLeft = toCompare.mLeft;
				else if (mRight < toCompare.mRight)
					mRight = toCompare.mRight;

				if (mMin_height > toCompare.mMin_height)
					mMin_height = toCompare.mMin_height;
				else if (mMax_height < toCompare.mMax_height)
					mMax_height = toCompare.mMax_height;
			}

			bool ObjectBase::mergeIfPossibleLeftToMe(ObjectBase &toCompare) noexcept
			{
				if (isMeargableToLeft(toCompare))
				{
					/// debug prints of object rects
					// printToConsole();
					// toCompare.printToConsole();
					mergeToMe(toCompare);
					return true;
				}
				return false;
			}

			std::vector<Pixel> &Object::getPixels() const noexcept { return mPixels; }

			size_t ObjectBase::getLeft() const noexcept { return mLeft; }

			size_t ObjectBase::getRight() const noexcept { return mRight; }

			size_t ObjectBase::getMinHeight() const noexcept { return mMin_height; }

			size_t ObjectBase::getMaxHeight() const noexcept { return mMax_height; }

			size_t ObjectBase::getPixelsCount() const noexcept { return mPixelsCount; }

			void ObjectBase::printToConsole() const noexcept
			{
				printf("Object Rectangle: %zd %zd   %zd %zd \n", mLeft, mRight, mMin_height,
					   mMax_height);
			}

			void ObjectBase::clearPixelsCount() noexcept
			{
				mPixelsCount = 0;
				mLeft = 0;
				mRight = 0;
				mMin_height = 0;
				mMax_height = 0;
			}

			ObjectRectangle::ObjectRectangle(const size_t row, const size_t col) : ObjectBase(row, col)
			{
			}

			void
			ObjectRectangle::addPixel(const size_t row, const size_t col)
			{
				if (mLeft > col)
					mLeft = col;
				else if (mRight < col)
					mRight = col;

				if (mMin_height > row)
					mMin_height = row;
				else if (mMax_height < row)
					mMax_height = row;
				++mPixelsCount;
			}
		} // namespace algorithm
	}	  // namespace analyze
} // namespace am