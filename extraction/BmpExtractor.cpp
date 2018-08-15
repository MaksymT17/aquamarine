
#include"BmpExtractor.h"
#include "../thirdparty/bitmap_reader/bitmap_image.hpp"
#include "../common/FileAccessException.hpp"

namespace recognition
{

	std::shared_ptr<common::Matrix<common::ColorChannels>> BmpExtractor::readFile(std::string& fileName)
	{
		bitmap_image image(fileName.c_str());

		if (!image)
		{
			printf("Error - Failed to open: input.bmp\n");
			std::string errMsg = "File '" + fileName + "' could not be found!";
			throw FileAccessException(errMsg);
		}


		const unsigned int height = image.height();
		const unsigned int width = image.width();

		// safe alllocation ?
		std::shared_ptr<common::Matrix<common::ColorChannels>> diffs(new common::Matrix<common::ColorChannels>(width, height));

		//todo: make multithreading there
		for (std::size_t y = 0; y < height; ++y)
		{
			for (std::size_t x = 0; x < width; ++x)
			{
				rgb_t colour;
				image.get_pixel(x, y, colour);

				auto &diffRef = *diffs.get();
				diffRef(y, x).r = colour.red;
				diffRef(y, x).g = colour.green;
				diffRef(y, x).b = colour.blue;
			}
		}
		return diffs;
	}

}