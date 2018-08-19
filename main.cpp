#include "analyze/AffinityComparer.h"
#include <stdio.h>
#include <memory>
#include"extraction/BmpExtractor.h"
#include"analyze/ThresholdDiffChecker.h"
#include"common/Context.hpp"
common::Context* common::Context::inst = nullptr;
int main() {
	using namespace recognition;
	using namespace common;
	
	recognition::BmpExtractor extractor;
	std::string base("sample_clean_2560x1440.BMP");
	std::string toCompare("sample_clean_mod_2560x1440.BMP");

	std::shared_ptr<Matrix<ColorChannels>> res = extractor.readFile(base);
	std::shared_ptr<Matrix<ColorChannels>> resMOd = extractor.readFile(toCompare);

	analyze::AffinityComparer checker(res);
	std::shared_ptr<Matrix<ColorChannelsDiff>> diffs = checker.compare(resMOd);

	analyze::ThresholdDiffChecker similarityCheck(20);
	float sim = similarityCheck.getAffinityPersent(diffs);
	printf("images similarity persent %f\n", sim *100.0f);
	return 0;
}
