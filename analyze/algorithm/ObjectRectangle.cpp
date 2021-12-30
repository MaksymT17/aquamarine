#include "ObjectRectangle.h"

namespace am
{
    namespace analyze
    {
        namespace algorithm
        {
            ObjectRectangle::ObjectRectangle(const size_t row, const size_t col)
                : ObjectBase(row, col)
            {
            }

            void ObjectRectangle::addPixel(const size_t row, const size_t col)
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
        }
    }
}