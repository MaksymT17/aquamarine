#pragma once
#include <cstdlib>

namespace am
{
    namespace analyze
    {
        namespace algorithm
        {

            class ObjectBase
			{
			public:
				explicit ObjectBase(const size_t row, const size_t col) : mPixelsCount(1),
																		  mLeft(col),
																		  mMin_height(row),
																		  mRight(col),
																		  mMax_height(row) {}
				bool isMeargableToVertical(ObjectBase &toCompare) const noexcept;
				bool mergeIfPossibleVerticalToMe(ObjectBase &toCompare) noexcept;

				size_t getLeft() const noexcept;
				size_t getRight() const noexcept;
				size_t getMinHeight() const noexcept;
				size_t getMaxHeight() const noexcept;
				size_t getPixelsCount() const noexcept;

				void printToConsole() const noexcept;
				void clearPixelsCount() noexcept;

			protected:
				void mergeToMe(ObjectBase &toCompare) noexcept;
				size_t mPixelsCount;
				size_t mLeft;
				size_t mMin_height;
				size_t mRight;
				size_t mMax_height;
			};
       
        }
    }
}