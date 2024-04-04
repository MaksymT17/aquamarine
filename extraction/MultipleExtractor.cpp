
#include "MultipleExtractor.h"
#include <future>
#include <algorithm>

#include "BmpExtractor.h"
#ifdef __unix__
#include "JpgExtractor.h"
#endif
#include "common/types/Color24b.hpp"
#include "common/exceptions/WrongFormatException.hpp"

namespace am
{
	namespace extraction
	{
		using namespace common::types;

		MultipleExtractor::MultipleExtractor(std::shared_ptr<am::common::Logger> &logger)
			: mLogger(logger)
		{
		}

		std::vector<Matrix<Color24b>> MultipleExtractor::readFiles(std::vector<std::string> &&fileNames)
		{
			std::vector<Matrix<Color24b>> result;
			result.reserve(fileNames.size());
			std::vector<std::future<Matrix<Color24b>>> futures;

			for (size_t i = 0; i < fileNames.size(); ++i)
			{
				std::string file_ext = fileNames[i].substr(fileNames[i].find_last_of(".") + 1);
				std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(),
							   [](unsigned char c)
							   { return std::tolower(c); });
				if(file_ext =="bmp"){
					futures.emplace_back(std::async(std::launch::deferred, BmpExtractor::readFile, fileNames[i]));
				}
#ifdef __unix__ // note: currently jpeg supported with Unix OS (with libjpeg)
				else if(file_ext =="jpg" || file_ext =="jpeg" || file_ext =="jpe") {
					futures.emplace_back(std::async(std::launch::deferred, JpgExtractor::readFile, fileNames[i]));
				}
#endif
				else{
					std::string errorMsg("WrongFormatException on data extraction from file(allowed jpeg/bmp)! File: ");
					errorMsg.append(fileNames[i]);
					throw am::common::exceptions::WrongFormatException(errorMsg);
				}

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