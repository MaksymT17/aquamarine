#include "analyze/AffinityComparer.h"
#include <stdio.h>
#include "extraction/MultipleBmpExtractor.h"
#include "extraction/BmpExtractor.h"
#include "extraction/BmpDrawer.hpp"
#include "analyze/ThresholdDiffChecker.h"
#include "analyze/algorithm/BfsObjectDetector.h"
#include "analyze/algorithm/DiffObjectDetector.h"
#include "analyze/algorithm/ObjectDetector.h"
#include "analyze/algorithm/movement/MovementDetector.h"
#include "analyze/algorithm/ImagePair.h"
#include "configuration/ConfigurationReader.hpp"
#include "common/Context.hpp"
#include <memory>

int main()
{
	using namespace am::common::types;
	using namespace am::analyze;

	std::shared_ptr<am::common::Logger> loggerPtr(new am::common::Logger("log.log"));
	am::extraction::MultipleBmpExtractor extractor(loggerPtr);
	std::string base("inputs/fhd_clean.BMP");
	std::string toCompare("inputs/fhd_5obj.BMP");
	std::vector<std::string> fileNames = { base, toCompare };

	//multiple reading of files
	std::vector<std::shared_ptr<Matrix<Color24b>>> data = extractor.readFiles(fileNames);

	std::shared_ptr<Matrix<Color24b>> res = data[0];
	std::shared_ptr<Matrix<Color24b>> resChange = data[1];

	std::shared_ptr<algorithm::ImagePair> pair(new algorithm::ImagePair(res, resChange));

	const size_t opt_threads = am::common::getOptimalThreadsCount();

	am::configuration::ConfigurationReader reader;

	auto conf = reader.getConfigurationFromFile("inputs/configuration.csv");

	//experimental - but, main feature currently, external review needed
	algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf, loggerPtr);
	algorithm::DiffObjectDetector diffDetector = algorithm::DiffObjectDetector(opt_threads, conf, loggerPtr);

	algorithm::DescObjects rects1 = detector.getObjectsRects(pair);

	for (auto& rect : rects1)
	{
		printf("row:%zd col:%zd    row:%zd col:%zd value:%zd\n", rect.getMinHeight(), rect.getLeft(), rect.getMaxHeight(), rect.getRight(), rect.getPixelsCount());
	}

	am::extraction::BmpDrawer drawer(fileNames[1]);
	algorithm::DescObjects rects2 = detector.getObjectsRects(pair);
	for (auto obj : rects2)
	{
		drawer.drawRectangle(obj.getLeft(), obj.getMinHeight(), obj.getRight(), obj.getMaxHeight());
	}
	drawer.save(std::string("compare_result.BMP"));
	return 0;
}