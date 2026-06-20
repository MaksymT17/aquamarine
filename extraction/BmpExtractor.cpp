#include "BmpExtractor.h"
#include "common/exceptions/FileAccessException.hpp"
#include "common/exceptions/AllocationException.hpp"
#include "thirdparty/stb/stb_image.h"
#include <future>
#include <vector>

namespace am::extraction
{
	using namespace common::types;

	Matrix<Color24b> BmpExtractor::readFile(const std::string &fileName)
	{
        int width, height, channels;
        unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);

		if (!data)
		{
			std::string errMsg = "File '" + fileName + "' could not be found or loaded!";
			throw common::exceptions::FileAccessException(errMsg);
		}

		Matrix<Color24b> mdata(width, height);

		for (int y = 0; y < height; ++y)
		{
            for (int x = 0; x < width; ++x)
            {
                int index = (y * width + x) * channels;
                if (channels >= 3) {
                    mdata(y, x) = {data[index], data[index + 1], data[index + 2]};
                } else if (channels == 1) {
                    mdata(y, x) = {data[index], data[index], data[index]};
                }
            }
		}

        stbi_image_free(data);
		return mdata;
	}
} // namespace am::extraction
