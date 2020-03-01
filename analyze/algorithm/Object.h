#pragma once
#include <vector>
#include <cstdlib>
#include <set>

namespace am {
	namespace analyze {
		namespace algorithm {
			// position of Pixel on image 
			struct Pixel {
				Pixel(size_t r, size_t c) :
					rowId(r),
					colId(c) {};
				size_t rowId;
				size_t colId;
			};

			struct Column {
				size_t left;
				size_t right;
			};

			// simple representatin of found object area on image, ismplified to rectangle bounds
			// base on diff pixels connected
			class Object
			{
			public:
				Object(std::vector<Pixel>& pixels);
				~Object() = default;

				bool isMeargableToLeft(Object& toCompare) const noexcept;
				bool mergeIfPossibleLeftToMe(Object& toCompare) noexcept;

				std::vector<Pixel>& getPixels() const noexcept;
				size_t getLeft() const noexcept;
				size_t getRight() const noexcept;
				size_t getMinHeight() const noexcept;
				size_t getMaxHeight() const noexcept;
				size_t getPixelsCount() const noexcept;

				void printToConsole() const noexcept;
				void clearPixelsCount()noexcept;

			private:
				void mergeToMe(Object& toCompare)noexcept;

				std::vector<Pixel>& mPixels;
				size_t mPixelsCount;
				size_t mLeft;
				size_t mMin_height;
				size_t mRight;
				size_t mMax_height;
			};

			namespace comparators
			{
				struct Ascending
				{
					bool operator() (const Object& l, const Object& r) const
					{
						return l.getPixelsCount() < r.getPixelsCount();
					}
				};

				struct Descending
				{
					bool operator() (const Object& l, const Object& r) const
					{
						return l.getPixelsCount() > r.getPixelsCount();
					}
				};

				struct Unordered
				{
					bool operator() (const Object& l, const Object& r) const
					{
						return true;
					}
				};
			}

			//template struct to make automatic ordering when new objects are inserting
			// why ? to make priority in collected objects
			// bigger should be first in collection
			// if not needed can be used Unordered comparator
			template <typename T>
			struct OrderedObjects
			{
				std::multiset<Object, T> objects;
			};

			using DescObjects = std::multiset<Object, comparators::Descending>;
		}
	}
}