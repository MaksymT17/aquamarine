#include"Object.h"
namespace aq {
	namespace analyze {
		namespace algorithm {

			Object::Object(std::vector<Pixel>& pixels) :
				min_width(pixels.begin()->colId),
				min_height(pixels.begin()->rowId),
				max_width(pixels.begin()->colId),
				max_height(pixels.begin()->rowId)

			{
				for (auto pixel : pixels)
				{
					if (min_width > pixel.colId)
						min_width = pixel.colId;
					else if (max_width < pixel.colId)
						max_width = pixel.colId;

					if (min_height > pixel.rowId)
						min_height = pixel.rowId;
					else if (max_height < pixel.rowId)
						max_height = pixel.rowId;
				}
			}

			bool Object::isMergableToRight(Object& toCompare) const
			{
				if (max_width + 1 == toCompare.getMinWidth()) {
					for (int i = min_height; i < max_height; ++i)
					{
						if (i == toCompare.min_height || i == toCompare.max_height)
							return true;
					}
				}
				return false;
			}
			void Object::mergeToMe(Object& toCompare)
			{
				if (min_width > toCompare.min_width)
					min_width = toCompare.min_width;
				else if (max_width < toCompare.max_width)
					max_width = toCompare.max_width;

				if (min_height > toCompare.min_height)
					min_height = toCompare.min_height;
				else if (max_height < toCompare.max_height)
					max_height = toCompare.max_height;

				//todo: set soome default value, means - to ignore
				toCompare.min_width = 0;
				toCompare.max_width = 0;
				toCompare.min_height = 0;
				toCompare.max_height = 0;
			}
			bool Object::mergeIfPossible(Object& toCompare)
			{
				if (isMergableToRight(toCompare))
				{
					mergeToMe(toCompare);
					return true;
				}
				return false;
			}
			size_t Object::getMinWidth() const
			{
				return min_width;
			}
			size_t Object::getMaxWidth() const
			{
				return max_width;
			}
			size_t Object::getMinHeight() const
			{
				return min_height;
			}
			size_t Object::getMaxHeight() const
			{
				return max_height;
			}
		}
	}
}