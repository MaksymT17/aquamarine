#include "Object.h"

namespace am
{
	namespace analyze
	{
		namespace algorithm
		{

			Object::Object(std::vector<Pixel> &pixels)
				: ObjectBase::ObjectBase(pixels.begin()->rowId, pixels.begin()->colId),
				  mPixels(pixels)
			{
				for (auto pixel = pixels.begin() + 1; pixel != pixels.end(); ++pixel)
				{
					if (mLeft > pixel->colId)
						mLeft = pixel->colId;
					else if (mRight < pixel->colId)
						mRight = pixel->colId;

					if (mMin_height > pixel->rowId)
						mMin_height = pixel->rowId;
					else if (mMax_height < pixel->rowId)
						mMax_height = pixel->rowId;
				}
				mPixelsCount = pixels.size();
			}

			std::vector<Pixel> &Object::getPixels() const noexcept { return mPixels; }

		} // namespace algorithm
	}	  // namespace analyze
} // namespace am