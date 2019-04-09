#pragma once
#include<vector>
#include<cstdlib>
#include <set>

namespace am {
	namespace analyze {
		namespace algorithm {
			// position of Pixel on image 
			struct Pixel {
				size_t rowId;
				size_t colId;
			};

			struct Column {
				size_t left;
				size_t right;
			};

			// simple representatin of found object area on image, ismplified to rectangle bounds
			// base on diff pixels connected
			class Object {
			public:
				Object(std::vector<Pixel>& pixels);
				virtual ~Object() = default;

				bool isMergableToRight(Object& toCompare) const;
				bool mergeIfPossible(Object& toCompare);

				size_t getMinWidth() const;
				size_t getMaxWidth() const;
				size_t getMinHeight() const;
				size_t getMaxHeight() const;
				size_t getValue() const;

			private:
				void mergeToMe(Object& toCompare);

				size_t mPixelsCount;
				size_t mMin_width;
				size_t mMin_height;
				size_t mMax_width;
				size_t mMax_height;
				size_t mValue;
			};

			//todo: move comparators to other place
			namespace comparators {
				struct Ascending {
					bool operator() (const Object& l, const Object& r) const
					{
						return l.getValue() < r.getValue();
					}
				};

				struct Descending {
					bool operator() (const Object& l, const Object& r) const
					{
						return l.getValue() > r.getValue();
					}
				};
				struct Unordered {
					bool operator() (const Object& l, const Object& r) const
					{
						return true;
					}
				};
			}

			//template struct to make automatic ordering when new objects are collected
			// why ? to make priority in collected objects
			// bigger should be first in collection
			// if not needed can be used vector instead
			template <typename T>
			struct OrderedObjects {
				std::multiset<Object, T> objects;
			};


			using DescObjects = std::multiset<Object, comparators::Descending>;
		}
	}
}