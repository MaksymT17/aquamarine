#include"Object.h"
#include<stdio.h>

namespace am {
	namespace analyze {
		namespace algorithm {

			Object::Object(std::vector<Pixel>& pixels) :
				mPixels(pixels),
				mPixelsCount(pixels.size()),
				mMin_width(pixels.begin()->colId),
				mMin_height(pixels.begin()->rowId),
				mMax_width(pixels.begin()->colId),
				mMax_height(pixels.begin()->rowId),
				mValue(pixels.size())
			{
				for (auto pixel : pixels)
				{
					if (mMin_width > pixel.colId)
						mMin_width = pixel.colId;
					else if (mMax_width < pixel.colId)
						mMax_width = pixel.colId;

					if (mMin_height > pixel.rowId)
						mMin_height = pixel.rowId;
					else if (mMax_height < pixel.rowId)
						mMax_height = pixel.rowId;
				}
			}

			bool Object::isMergableToRight(Object& rightObj) const
			{
				if (mMax_width >= rightObj.getMinWidth())
				{
					if (!(getMinHeight() > rightObj.getMaxHeight() || getMinWidth() > rightObj.getMaxWidth()))
						return true;
				}
				return false;
			}

			void Object::mergeToMe(Object& toCompare)
			{
				mPixelsCount += toCompare.mPixelsCount;

				if (mMin_width > toCompare.mMin_width)
					mMin_width = toCompare.mMin_width;
				else if (mMax_width < toCompare.mMax_width)
					mMax_width = toCompare.mMax_width;

				if (mMin_height > toCompare.mMin_height)
					mMin_height = toCompare.mMin_height;
				else if (mMax_height < toCompare.mMax_height)
					mMax_height = toCompare.mMax_height;

				// reset default value, means - to ignore
				/// why?: removing of vector element seems costly
				toCompare.mMin_width = 0;
				toCompare.mMax_width = 0;
				toCompare.mMin_height = 0;
				toCompare.mMax_height = 0;
			}
			bool Object::mergeIfPossible(Object& toCompare)
			{
				if (isMergableToRight(toCompare))
				{
					printf("----------------------\n");
					printToConsole();
					toCompare.printToConsole();
					printf("----------------------\n");
					mergeToMe(toCompare);
					mValue += toCompare.getValue();
					return true;
				}
				return false;
			}
			std::vector<Pixel>& Object::getPixels() const
			{
				return mPixels;
			}
			size_t Object::getMinWidth() const
			{
				return mMin_width;
			}
			size_t Object::getMaxWidth() const
			{
				return mMax_width;
			}
			size_t Object::getMinHeight() const
			{
				return mMin_height;
			}
			size_t Object::getMaxHeight() const
			{
				return mMax_height;
			}
			size_t Object::getValue() const
			{
				return mValue;
			}
			void Object::printToConsole() const
			{
				printf("Object Rectangle: %d %d   %d %d \n", mMin_width, mMax_width, mMin_height, mMax_height);
			}
		}
	}
}