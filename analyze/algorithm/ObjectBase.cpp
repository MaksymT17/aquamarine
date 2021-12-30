#include "ObjectBase.h"
#include <iostream>

namespace am
{
    namespace analyze
    {
        namespace algorithm
        {

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

            size_t ObjectBase::getLeft() const noexcept { return mLeft; }

			size_t ObjectBase::getRight() const noexcept { return mRight; }

			size_t ObjectBase::getMinHeight() const noexcept { return mMin_height; }

			size_t ObjectBase::getMaxHeight() const noexcept { return mMax_height; }

			size_t ObjectBase::getPixelsCount() const noexcept { return mPixelsCount; }

			void ObjectBase::printToConsole() const noexcept
			{
				std::cout << "Object Rectangle: " << mLeft << " " << mRight << " " << mMin_height << "  " << mMax_height << " " << std::endl;
			}

			void ObjectBase::clearPixelsCount() noexcept
			{
				mPixelsCount = 0;
				mLeft = 0;
				mRight = 0;
				mMin_height = 0;
				mMax_height = 0;
			}
        }
    }
}