#pragma once
#include <cstdlib>
#include <stdint.h>
#include <vector>

namespace am
{
	namespace common
	{
		namespace types
		{

			template <class T>
			class Matrix
			{
			public:
				Matrix(const size_t width, const size_t height)
					: mWidth(width), mHeight(height), mData(height, std::vector<T>(width))
				{
				}
				~Matrix() = default;

				size_t getWidth() const noexcept { return mWidth; }
				size_t getHeight() const noexcept { return mHeight; }

				T &operator()(size_t i, size_t j)
				{
					return mData[i][j];
				}

				T operator()(size_t i, size_t j) const
				{
					return mData[i][j];
				}

			private:
				const size_t mWidth;
				const size_t mHeight;

				std::vector<std::vector<T>> mData;
			};

			using MatrixU16 = Matrix<uint16_t>;
		} // namespace types
	}	  // namespace common
} // namespace am