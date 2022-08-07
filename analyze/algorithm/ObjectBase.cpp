#include "ObjectBase.h"
#include <iostream>

namespace am
{
    namespace analyze
    {
        namespace algorithm
        {

            bool ObjectBase::isMeargableToVertical(ObjectBase &leftObj) const noexcept
			{
				if (leftObj.getMaxHeight() + 1 == mMin_height)
				{
					// check if ranges are intersecting, if yes can be mergedi nto one object
					if (getLeft() <= leftObj.getLeft() && leftObj.getLeft() <= getRight() ||
						getLeft() <= leftObj.getRight() && leftObj.getRight() <= getRight() ||
						getLeft() >= leftObj.getLeft() && getLeft() <=leftObj.getRight() || 
						leftObj.getLeft() >= getLeft() && leftObj.getLeft() <= getRight())
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

			bool ObjectBase::mergeIfPossibleVerticalToMe(ObjectBase &toCompare) noexcept
			{
				if (isMeargableToVertical(toCompare))
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