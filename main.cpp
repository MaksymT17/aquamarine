#include "analyze/AffinityComparer.h"
#include <stdio.h>
#include <memory>
#include"extraction/BmpExtractor.h"
#include"extraction/MultipleBmpExtractor.h"
#include"analyze/ThresholdDiffChecker.h"
#include"analyze/algorithm/ObjectDetector.h"
#include"common/Context.hpp"
#include <future>

common::Context* common::Context::inst = nullptr;

using namespace recognition;
using namespace common;

int main()
{
	recognition::MultipleBmpExtractor extractor;
	std::string base("image_inputs/fhd_clean.BMP");
	std::string toCompare("image_inputs/fhd_5obj.BMP");
	std::vector<std::string> fileNames = { base, toCompare };

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

	//experimental - but, main feature currently, external review needed
	analyze::algorithm::ObjectDetector detector = analyze::algorithm::ObjectDetector(Context::getInstance()->getOpimalThreadsCount());
	std::vector<analyze::algorithm::Object> rects = detector.getObjectsRects(diffs);

	//clean up all used data
	common::Context::release();
	return 0;
}
