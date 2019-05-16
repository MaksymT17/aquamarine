#include"Object.h"
#include<stdio.h>

namespace am {
	namespace analyze {
		namespace algorithm {

			Object::Object(std::vector<Pixel>& pixels) :
				mPixels(pixels),
				mPixelsCount(pixels.size()),
				mLeft(pixels.begin()->colId),
				mMin_height(pixels.begin()->rowId),
				mRight(pixels.begin()->colId),
				mMax_height(pixels.begin()->rowId)
			{
				for (const auto& pixel : pixels)
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

			bool Object::isMergableToRight(Object& rightObj) const
			{
				if (mRight >= rightObj.getLeft())
				{
					if (!(getMinHeight() > rightObj.getMaxHeight() || getLeft() > rightObj.getRight()))
						return true;
				}
				return false;
			}

			void Object::mergeToMe(Object& toCompare)
			{
				mPixelsCount += toCompare.mPixelsCount;

				if (mLeft > toCompare.mLeft)
					mLeft = toCompare.mLeft;
				else if (mRight < toCompare.mRight)
					mRight = toCompare.mRight;

				if (mMin_height > toCompare.mMin_height)
					mMin_height = toCompare.mMin_height;
				else if (mMax_height < toCompare.mMax_height)
					mMax_height = toCompare.mMax_height;

			}
			bool Object::mergeIfPossible(Object& toCompare)
			{
				if (isMergableToRight(toCompare))
				{
					printToConsole();
					toCompare.printToConsole();
					mergeToMe(toCompare);
					return true;
				}
				return false;
			}
			const std::vector<Pixel>& Object::getPixels() const
			{
				return mPixels;
			}
			size_t Object::getLeft() const
			{
				return mLeft;
			}
			size_t Object::getRight() const
			{
				return mRight;
			}
			size_t Object::getMinHeight() const
			{
				return mMin_height;
			}
			size_t Object::getMaxHeight() const
			{
				return mMax_height;
			}
			size_t Object::getPixelsCount() const
			{
				return mPixelsCount;
			}
			void Object::printToConsole() const
			{
				printf("Object Rectangle: %d %d   %d %d \n", mLeft, mRight, mMin_height, mMax_height);
			}
		}
	}
}