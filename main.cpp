#include "analyze/AffinityComparer.h"
#include <stdio.h>
#include "extraction/MultipleBmpExtractor.h"
#include "analyze/ThresholdDiffChecker.h"
#include "analyze/algorithm/ObjectDetector.h"
#include "analyze/algorithm/ImagePair.h"
#include "common/Context.hpp"
#include "configuration/ConfigurationReader.hpp"

am::common::Context* am::common::Context::inst = nullptr;

// below are set of tests better way put this tests in UT,
// and run here only required detection logic

int main()
{
	using namespace am::common::types;
	using namespace am::analyze;
	am::extraction::MultipleBmpExtractor extractor;
	std::string base("inputs/fhd_clean.BMP");
	std::string toCompare("inputs/fhd_5obj.BMP");
	std::vector<std::string> fileNames = { base, toCompare };

	//multiple reading of files
	std::vector<std::shared_ptr<Matrix<ColorChannels>>> data = extractor.readFiles(fileNames);

	std::shared_ptr<Matrix<ColorChannels>> res = data[0];
	std::shared_ptr<Matrix<ColorChannels>> resChange = data[1];

	std::shared_ptr<algorithm::ImagePair> pair(new algorithm::ImagePair(res, resChange));

	//find of resulting matrix of diffs
	AffinityComparer checker(res);
	std::shared_ptr<Matrix<ColorChannelsDiff>> diffs = checker.compare(resChange);

	//check how images similar reagrding to affinity percent
	//according to last tests in day light threshold should be near to 200
	ThresholdDiffChecker similarityCheck(200);

	float sim = similarityCheck.getAffinityPersent(diffs);

	printf("images similarity persent %f\n", sim *100.0f);

	//experimental - but, main feature currently, external review needed
	algorithm::ObjectDetector detector = algorithm::ObjectDetector();

	algorithm::DescObjects rects = detector.getObjectsRects(diffs);
	algorithm::DescObjects rects1 = detector.getObjectsRects(pair);

	for (auto& rect : rects1.objects)
	{
		printf("row:%zd col:%zd    row:%zd col:%zd value:%zd\n", rect.getMinHeight(), rect.getMinWidth(), rect.getMaxHeight(), rect.getMaxWidth(), rect.getValue());
	}

	//clean up all used data
	am::common::Context::release();
	return 0;
}