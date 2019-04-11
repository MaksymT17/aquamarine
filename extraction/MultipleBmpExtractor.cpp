
#include "MultipleBmpExtractor.h"
#include <future>
#include "BmpExtractor.h"
#include "common/types/ColorChannels.hpp"

namespace am
{
	namespace extraction
	{
		using namespace common::types;
		std::shared_ptr<Matrix<ColorChannels>> getFileData(std::string file)
		{
			return BmpExtractor::readFile(file);
		}

		MultipleBmpExtractor::MultipleBmpExtractor()
			: mLogger(new am::common::Logger())
		{
		}

		std::vector<std::shared_ptr<Matrix<ColorChannels>>> MultipleBmpExtractor::readFiles(std::vector<std::string> fileNames)
		{
			std::vector<std::shared_ptr<Matrix<ColorChannels>>> result;
			result.reserve(fileNames.size());
			std::vector<std::future<std::shared_ptr<Matrix<ColorChannels>>>> futures;

			for (size_t i = 0; i < fileNames.size(); ++i)
			{
				mLogger->info("BmpExtractor::readFile file:%s.", fileNames[i].c_str());
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