
#include "MultipleBmpExtractor.h"
#include <future>
#include "BmpExtractor.h"
#include "common/types/Color24b.hpp"

namespace am
{
	namespace extraction
	{
		using namespace common::types;

		MultipleBmpExtractor::MultipleBmpExtractor(std::shared_ptr<am::common::Logger> &logger)
			: mLogger(logger)
		{
		}

		std::vector<Matrix<Color24b>> MultipleBmpExtractor::readFiles(std::vector<std::string> &fileNames)
		{
			std::vector<Matrix<Color24b>> result;
			result.reserve(fileNames.size());
			std::vector<std::future<Matrix<Color24b>>> futures;

			for (size_t i = 0; i < fileNames.size(); ++i)
			{
				futures.emplace_back(std::async(std::launch::async, BmpExtractor::readFile, fileNames[i]));
				mLogger->info("Reading of file:%s has been added in extraction queue.", fileNames[i].c_str());
			}

			for (auto &e : futures)
			{
				result.emplace_back(e.get());
			}
			return result;
		}
	}
}