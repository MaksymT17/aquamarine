#pragma once
#include <cstdlib>

namespace am::analyze::algorithm
{

	class ObjectBase
	{
	public:
		explicit ObjectBase(const size_t row, const size_t col) : mPixelsCount(0),
																  mLeft(col),
																  mMin_height(row),
																  mRight(col),
																  mMax_height(row) {}
		~ObjectBase() = default;

		bool isMeargableToVertical(const ObjectBase &toCompare) const noexcept;
		bool mergeIfPossibleVerticalToMe(const ObjectBase &toCompare) noexcept;

		size_t getLeft() const noexcept;
		size_t getRight() const noexcept;
		size_t getMinHeight() const noexcept;
		size_t getMaxHeight() const noexcept;
		size_t getPixelsCount() const noexcept;

		void printToConsole() const noexcept;
		void clearPixelsCount() noexcept;

	protected:
		void mergeToMe(const ObjectBase &toCompare) noexcept;
		size_t mPixelsCount;
		size_t mLeft;
		size_t mMin_height;
		size_t mRight;
		size_t mMax_height;
	};

} // namespace am::analyze::algorithm
