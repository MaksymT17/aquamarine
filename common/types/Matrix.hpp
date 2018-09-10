#pragma once
#include <vector>
#include <cstdlib>

namespace am {
	namespace common {
		namespace types {

			template<class T>
			class Matrix
			{
			public:
				Matrix(size_t width, size_t height)
					: mWidth(width),
					mHeight(height),
					mData(width * height)
				{
				}

				size_t getWidth()const { return mWidth; }
				size_t getHeight()const { return mHeight; }

				T& operator()(size_t i, size_t j)
				{
					return mData[i * mWidth + j];
				}

				T operator()(size_t i, size_t j) const
				{
					return mData[i * mWidth + j];
				}

			private:
				size_t mWidth;
				size_t mHeight;
				std::vector<T> mData;
			};

		}
	}
}