#include "analyze/AffinityComparer.h"
#include <stdio.h>
#include <memory>
#include"extraction/BmpExtractor.h"
#include"analyze/ThresholdDiffChecker.h"

int main() {
	using namespace recognition;
	using namespace common;

	recognition::BmpExtractor extractor;
	std::string base("sample_clean_2560x1440.BMP");
	std::string toCompare("sample_clean_mod_2560x1440.BMP");

	std::shared_ptr<common::Matrix<common::ColorChannels>> res = extractor.readFile(base);
	std::shared_ptr<common::Matrix<common::ColorChannels>> resMOd = extractor.readFile(toCompare);

	analyze::AffinityComparer checker(res);
	std::shared_ptr<Matrix<ColorChannelsDiff>> diffs = checker.compare(resMOd);

	analyze::ThresholdDiffChecker similarityCheck(20);
	float sim = similarityCheck.getAffinityPersent(diffs);
	printf("images similarity persent %f\n", sim);
	return 0;
}
