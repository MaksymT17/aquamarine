#pragma once
#include <cstdlib>
#include <stdint.h>
#include <vector>

namespace am {
namespace common {
namespace types {

template <class T> class Matrix {
public:
  Matrix(const size_t width, const size_t height)
      : mWidth(width), mHeight(height), mData(height, std::vector<T>(width)) {
  
	  size_t a =sizeof(mData);
  }
  virtual ~Matrix() = default;

  size_t getWidth() const { return mWidth; }
  size_t getHeight() const { return mHeight; }

  T &operator()(size_t i, size_t j) { return mData[i][j]; }

  T operator()(size_t i, size_t j) const { return mData[i][j]; }

private:
  const size_t mWidth;
  const size_t mHeight;
  // todo:find out why: VS compiler differs from others and mData can throw exc
  // on destroy in meantime under linux (GNU), binary behaves fine
  std::vector<std::vector<T>> mData;
};

using MatrixU16 = Matrix<uint16_t>;
} // namespace types
} // namespace common
} // namespace am