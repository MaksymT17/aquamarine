#pragma once
#include <vector>
#include <cstdlib>
#include <set>

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

			struct Column
			{
				size_t left;
				size_t right;
			};

			class ObjectBase
			{
			public:
				explicit ObjectBase(const size_t row, const size_t col) : mPixelsCount(1),
																		  mLeft(col),
																		  mMin_height(row),
																		  mRight(col),
																		  mMax_height(row) {}
				bool isMeargableToLeft(ObjectBase &toCompare) const noexcept;
				bool mergeIfPossibleLeftToMe(ObjectBase &toCompare) noexcept;

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

			// Rectangled area of object
			class ObjectRectangle : public ObjectBase
			{
			public:
				//ObjectRectangle() = default;
				ObjectRectangle(const size_t row, const size_t col);
				void addPixel(const size_t row, const size_t col);
			};

			namespace comparators
			{
				struct Ascending
				{
					bool operator()(const Object &l, const Object &r) const
					{
						return l.getPixelsCount() < r.getPixelsCount();
					}
				};

				struct Descending
				{
					bool operator()(const Object &l, const Object &r) const
					{
						return l.getPixelsCount() > r.getPixelsCount();
					}
					bool operator()(const ObjectRectangle &l, const ObjectRectangle &r) const
					{
						return l.getPixelsCount() > r.getPixelsCount();
					}
				};

				struct Unordered
				{
					bool operator()(const Object &l, const Object &r) const
					{
						return true;
					}
				};
			}

			// template struct to make automatic ordering when new objects are inserting
			//  why ? to make priority in collected objects
			//  bigger should be first in collection
			//  if not needed can be used Unordered comparator
			template <typename T>
			struct OrderedObjects
			{
				std::multiset<Object, T> objects;
			};

			using DescObjects = std::multiset<ObjectRectangle, comparators::Descending>;
		}
	}
}