#include "analyze/AffinityComparer.h"
#include <stdio.h>
#include <memory>
#include"extraction/BmpExtractor.h"
#include"analyze/TresholdDiffChecker.h"

int main() {
	using namespace recognition;
	using namespace common;

	recognition::BmpExtractor extractor;
	std::shared_ptr<common::Matrix<common::ColorChannels>> res = extractor.readFile(std::string("sample_clean_2560x1440.BMP"));
	std::shared_ptr<common::Matrix<common::ColorChannels>> resMOd = extractor.readFile(std::string("sample_clean_mod_2560x1440.BMP"));
	analyze::AffinityComparer checker(res);
	std::shared_ptr<Matrix<ColorChannelsDiff>> diffs = checker.compare(resMOd);
	
	analyze::TresholdDiffChecker similarityCheck(20);
	float sim = similarityCheck.getAffinityPersent(diffs);
	printf("images similarity persent %f\n", sim);
	return 0;
}
