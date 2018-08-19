
#include "MultipleBmpExtractor.h"
#include <future>
#include"BmpExtractor.h"

namespace recognition
{
	std::shared_ptr<common::Matrix<common::ColorChannels>> getFileData(std::string file)
	{
		recognition::BmpExtractor extractor;
		return extractor.readFile(file);
	}

	std::vector<std::shared_ptr<common::Matrix<common::ColorChannels>>> MultipleBmpExtractor::readFiles(std::vector<std::string> fileNames)
	{
		std::vector<std::shared_ptr<common::Matrix<common::ColorChannels>>> result;
		
		std::vector<std::future<std::shared_ptr<common::Matrix<common::ColorChannels>>>> futures;

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