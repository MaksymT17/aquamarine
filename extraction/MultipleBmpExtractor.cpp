
#include "MultipleBmpExtractor.h"
#include <future>
#include "BmpExtractor.h"
#include "common/types/Color24b.hpp"

namespace am
{
	namespace extraction
	{
		using namespace common::types;
		std::shared_ptr<Matrix<Color24b>> getFileData(std::string& file)
		{
			return BmpExtractor::readFile(file);
		}

		MultipleBmpExtractor::MultipleBmpExtractor(std::shared_ptr<am::common::Logger>& logger)
			: mLogger(logger)
		{
		}

		std::vector<std::shared_ptr<Matrix<Color24b>>> MultipleBmpExtractor::readFiles(std::vector<std::string>& fileNames)
		{
			std::vector<std::shared_ptr<Matrix<Color24b>>> result;
			result.reserve(fileNames.size());
			std::vector<std::future<std::shared_ptr<Matrix<Color24b>>>> futures;

			for (size_t i = 0; i < fileNames.size(); ++i)
			{
				mLogger->info("BmpExtractor::readFile file:%s.", fileNames[i].c_str());
				futures.emplace_back(std::async(std::launch::async, getFileData, fileNames[i]));
			}

			for (auto &e : futures)
			{
				result.emplace_back(e.get());
			}
			return result;
		}
	}
}