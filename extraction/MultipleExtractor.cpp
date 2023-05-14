
#include "MultipleExtractor.h"
#include <future>
#include "BmpExtractor.h"
#include "JpgExtractor.h"
#include "common/types/Color24b.hpp"

namespace am
{
	namespace extraction
	{
		using namespace common::types;

		MultipleExtractor::MultipleExtractor(std::shared_ptr<am::common::Logger> &logger, bool is_bmp)
			: mLogger(logger),
			mIsForBmp(is_bmp)
		{
		}

		std::vector<Matrix<Color24b>> MultipleExtractor::readFiles(std::vector<std::string> &&fileNames)
		{
			std::vector<Matrix<Color24b>> result;
			result.reserve(fileNames.size());
			std::vector<std::future<Matrix<Color24b>>> futures;

			for (size_t i = 0; i < fileNames.size(); ++i)
			{
				if(mIsForBmp)
					futures.emplace_back(std::async(std::launch::deferred, BmpExtractor::readFile, fileNames[i]));
				else
					futures.emplace_back(std::async(std::launch::deferred, JpgExtractor::readFile, fileNames[i]));

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