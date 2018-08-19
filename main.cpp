#include "analyze/AffinityComparer.h"
#include <stdio.h>
#include <memory>
#include"extraction/BmpExtractor.h"
#include"extraction/MultipleBmpExtractor.h"
#include"analyze/ThresholdDiffChecker.h"
#include"common/Context.hpp"
#include <future>

common::Context* common::Context::inst = nullptr;
using namespace recognition;
using namespace common;

std::shared_ptr<Matrix<ColorChannels>> getFileData(std::string& file)
{
	recognition::BmpExtractor extractor;
	return extractor.readFile(file);
}

int main()
{
	recognition::MultipleBmpExtractor extractor;
	std::string base("image_inputs/sample_clean_2560x1440.BMP");
	std::string toCompare("image_inputs/sample_clean_mod_2560x1440.BMP");
	std::vector<std::string> fileNames = { base,toCompare };

	//multiple reading of files
	std::vector<std::shared_ptr<Matrix<ColorChannels>>> data = extractor.readFiles(fileNames);

	std::shared_ptr<Matrix<ColorChannels>> res = data[0];
	std::shared_ptr<Matrix<ColorChannels>> resMOd = data[1];

	//find of rsulting matrix of diffs
	analyze::AffinityComparer checker(res);
	std::shared_ptr<Matrix<ColorChannelsDiff>> diffs = checker.compare(resMOd);

	//check how images similar reagrding to affinity percent
	analyze::ThresholdDiffChecker similarityCheck(20);
	float sim = similarityCheck.getAffinityPersent(diffs);
	printf("images similarity persent %f\n", sim *100.0f);

	return 0;
}
