
#include "BmpExtractor.h"
#include "common/exceptions/FileAccessException.hpp"
#include "common/exceptions/AllocationException.hpp"
#include "thirdparty/bitmap_reader/bitmap_image.hpp"
#include <future>
#include <vector>

namespace am
{
	namespace extraction
	{
		using namespace common::types;
		void readRowFromImageSource(const size_t rowId, const size_t width, bitmap_image& image, Matrix<Color24b>& colors)
		{
			for (size_t x = 0; x < width; ++x)
			{
				Color24b& channels = colors(rowId, x);
				image.get_pixel(x, rowId, reinterpret_cast<rgb_t&>(channels));
			}
		}

		Matrix<Color24b> BmpExtractor::readFile(const std::string& fileName)
		{
			bitmap_image image(fileName.c_str());

			if (!image)
			{
				std::string errMsg = "File '" + fileName + "' could not be found!";
				throw common::exceptions::FileAccessException(errMsg);
			}

			const unsigned int height = image.height();
			const unsigned int width = image.width();

			Matrix<Color24b> data(width, height);

			for (std::size_t y = 0; y < height; ++y)
			{
				readRowFromImageSource(y, width, image, data);
			}

			return data;
		}
	}
}