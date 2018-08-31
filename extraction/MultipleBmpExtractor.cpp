
#include "MultipleBmpExtractor.h"
#include <future>
#include "BmpExtractor.h"
#include "common/types/ColorChannels.hpp"

namespace aq
{
	namespace extraction
	{
		using namespace common::types;
		std::shared_ptr<Matrix<ColorChannels>> getFileData(std::string file)
		{
			return BmpExtractor::readFile(file);
		}

		std::vector<std::shared_ptr<Matrix<ColorChannels>>> MultipleBmpExtractor::readFiles(std::vector<std::string> fileNames)
		{
			std::vector<std::shared_ptr<Matrix<ColorChannels>>> result;

			std::vector<std::future<std::shared_ptr<Matrix<ColorChannels>>>> futures;

			for (size_t i = 0; i < fileNames.size(); ++i)
			{
				futures.push_back(std::async(getFileData, fileNames[i]));
			}

			for (auto &e : futures)
			{
				result.push_back(e.get());
			}
			return result;
		}
	}
}