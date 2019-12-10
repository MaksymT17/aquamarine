#include "Object.h"
#include <stdio.h>

namespace am {
	namespace analyze {
		namespace algorithm {

			Object::Object(std::vector<Pixel> &pixels)
				: mPixels(pixels), mPixelsCount(pixels.size()),
				mLeft(pixels.begin()->colId), mMin_height(pixels.begin()->rowId),
				mRight(pixels.begin()->colId), mMax_height(pixels.begin()->rowId)
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

			bool Object::isMeargableToLeft(Object & leftObj) const noexcept
			{
				if (leftObj.getRight() + 1 == mLeft) {
					if (getMinHeight() <= leftObj.getMinHeight() && leftObj.getMinHeight() <= getMaxHeight() ||
						getMinHeight() <= leftObj.getMaxHeight() && leftObj.getMaxHeight() <= getMaxHeight())
						return true;
				}
				return false;
			}

			void Object::mergeToMe(Object &toCompare) noexcept
			{
				mPixelsCount += toCompare.mPixelsCount;

				//note: next line can be time costly, but important in Movement detection
				//mPixels.insert(mPixels.end(), toCompare.getPixels().begin(), toCompare.getPixels().end());

				if (mLeft > toCompare.mLeft)
					mLeft = toCompare.mLeft;
				else if (mRight < toCompare.mRight)
					mRight = toCompare.mRight;

				if (mMin_height > toCompare.mMin_height)
					mMin_height = toCompare.mMin_height;
				else if (mMax_height < toCompare.mMax_height)
					mMax_height = toCompare.mMax_height;
			}

			bool Object::mergeIfPossibleLeftToMe(Object &toCompare) noexcept
			{
				if (isMeargableToLeft(toCompare)) {
					/// debug prints of object rects
					// printToConsole();
					// toCompare.printToConsole();
					mergeToMe(toCompare);
					return true;
				}
				return false;
			}

			std::vector<Pixel> &Object::getPixels() const noexcept { return mPixels; }

			size_t Object::getLeft() const noexcept { return mLeft; }

			size_t Object::getRight() const noexcept { return mRight; }

			size_t Object::getMinHeight() const noexcept { return mMin_height; }

			size_t Object::getMaxHeight() const noexcept { return mMax_height; }

			size_t Object::getPixelsCount() const noexcept { return mPixelsCount; }

			void Object::printToConsole() const noexcept {
				printf("Object Rectangle: %zd %zd   %zd %zd \n", mLeft, mRight, mMin_height,
					mMax_height);
			}

			void Object::clearPixelsCount() noexcept
			{
				mPixelsCount = 0;
				//mLeft = 0;
				//mRight = 0;
				//mMin_height = 0;
				//mMax_height = 0;
				//mPixels.empty();
			}
		} // namespace algorithm
	} // namespace analyze
} // namespace am