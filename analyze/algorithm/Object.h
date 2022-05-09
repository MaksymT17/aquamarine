#pragma once
#include <vector>
#include <cstdlib>
#include <set>
#include "ObjectBase.h"

namespace am
{
	namespace analyze
	{
		namespace algorithm
		{
			// position of Pixel on image
			struct Pixel
			{
				Pixel(size_t r, size_t c) : rowId(r),
											colId(c){};
				size_t rowId;
				size_t colId;
			};

			struct ImageRowSegment
			{
				size_t start;
				size_t end;
			};

			// full representation of found object area on image, has rectangle bounds
			// and coordinates of pixels
			class Object : public ObjectBase
			{
			public:
				Object(std::vector<Pixel> &pixels);
				~Object() = default;
				std::vector<Pixel> &getPixels() const noexcept;

			private:
				std::vector<Pixel> &mPixels;
			};

			// template struct to make automatic ordering when new objects are inserting
			//  why ? to make priority in collected objects
			//  bigger should be first in collection
			//  if not needed can be used Unordered comparator
			template <typename T>
			struct OrderedObjects
			{
				std::multiset<Object, T> objects;
			};
		}
	}
}