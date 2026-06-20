
#include "MultipleExtractor.h"
#include <future>
#include <algorithm>
#include <spdlog/spdlog.h>

#include "common/types/Color24b.hpp"
#include "common/exceptions/WrongFormatException.hpp"

namespace am
{
	namespace extraction
	{
		using namespace common::types;

		MultipleExtractor::MultipleExtractor(std::unique_ptr<ExtractorFactory> factory) : m_factory(std::move(factory))
		{
		}

		std::vector<Matrix<Color24b>> MultipleExtractor::readFiles(std::vector<std::string> &&fileNames)
		{
			std::vector<Matrix<Color24b>> result;
			result.reserve(fileNames.size());
			std::vector<std::future<Matrix<Color24b>>> futures;

			for (size_t i = 0; i < fileNames.size(); ++i)
			{
				spdlog::info("MultipleExtractor readFile {}", fileNames[i].c_str());
				std::string file_ext = fileNames[i].substr(fileNames[i].find_last_of(".") + 1);
				std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(),
							   [](unsigned char c)
							   { return std::tolower(c); });
                
                auto extractorFunc = m_factory->getExtractor(file_ext);
                futures.emplace_back(std::async(std::launch::deferred, [func = std::move(extractorFunc), file = fileNames[i]]() {
                    return func(file);
                }));

				spdlog::info("Reading of file:{} has been added in extraction queue.", fileNames[i].c_str());
			}

			for (auto &e : futures)
			{
				result.emplace_back(e.get());
			}
			return result;
		}
	}
}