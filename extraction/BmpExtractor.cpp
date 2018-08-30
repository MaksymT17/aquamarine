
#include "BmpExtractor.h"
#include "thirdparty/bitmap_reader/bitmap_image.hpp"
#include "common/FileAccessException.hpp"
#include "common/AllocationException.hpp"
#include<future>
#include<vector>

namespace recognition
{
	void readRowFromImageSource(const size_t rowId, const size_t width, bitmap_image& image, common::Matrix<common::ColorChannels>& colors)
	{
		for (std::size_t x = 0; x < width; ++x)
		{
			common::ColorChannels& channels = colors(rowId, x);
			image.get_pixel(x, rowId, reinterpret_cast<rgb_t&>(channels)); // cast there: unsinged chat -> uint8_t
		}
	}

	std::shared_ptr<common::Matrix<common::ColorChannels>> BmpExtractor::readFile(std::string& fileName)
	{
		bitmap_image image(fileName.c_str());

		if (!image)
		{
			std::string errMsg = "File '" + fileName + "' could not be found!";
			throw common::FileAccessException(errMsg);
		}

		const unsigned int height = image.height();
		const unsigned int width = image.width();
		
		std::shared_ptr<common::Matrix<common::ColorChannels>> data(new common::Matrix<common::ColorChannels>(width, height));

		auto& colorsRef = *data.get();

		for (std::size_t y = 0; y < height; ++y)
		{
			readRowFromImageSource(y, width, image, colorsRef);
		}
		return data;
	}

}