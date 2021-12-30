#pragma once
#include <cstdlib>
#include <set>
#include "ObjectBase.h"

namespace am
{
    namespace analyze
    {
        namespace algorithm
        {
            class ObjectRectangle : public ObjectBase
            {
            public:
                ObjectRectangle(const size_t row, const size_t col);
                void addPixel(const size_t row, const size_t col);
            };

            namespace comparators
			{

				struct Descending
				{
					bool operator()(const ObjectRectangle &l, const ObjectRectangle &r) const
					{
						return l.getPixelsCount() > r.getPixelsCount();
					}
				};

			}

			using DescObjects = std::multiset<ObjectRectangle, comparators::Descending>;
        }
    }
}