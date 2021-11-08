#include "analyze/AffinityComparer.h"
#include "analyze/ThresholdDiffChecker.h"
#include "analyze/algorithm/BfsObjectDetector.h"
#include "analyze/algorithm/DiffObjectDetector.h"
#include "analyze/algorithm/ImagePair.h"
#include "analyze/algorithm/ObjectDetector.h"
#include "analyze/algorithm/movement/MovementDetector.h"
#include "common/Context.hpp"
#include "configuration/ConfigurationReader.hpp"
#include "extraction/BmpDrawer.hpp"
#include "extraction/BmpExtractor.h"
#include "extraction/MultipleBmpExtractor.h"
#include <memory>
#include <stdio.h>

int main(int argc, char * argv[]) 
{
	using namespace am::common::types;
	using namespace am::analyze;

	std::shared_ptr<am::common::Logger> loggerPtr(std::make_shared<am::common::Logger>("log.log"));

	am::extraction::MultipleBmpExtractor extractor(loggerPtr);

	std::string base_img_path;
	std::string cmp_img_path;
	const char* conf_path;
	const char* out_img_path;

	if (argc == 5)
	{
		std::cout << "Parse user-defined parameters." << std::endl;
		base_img_path = std::string(argv[1]);
		cmp_img_path = std::string(argv[2]);
		conf_path = argv[3];
		out_img_path = argv[4];
	}
	else
	{
		std::cout << "Use default parameters" << std::endl;
		base_img_path = "inputs/rs_1.bmp";
		cmp_img_path = "inputs/rs_2.bmp";
		conf_path = "inputs/configuration.csv";
		out_img_path = "compare_result.bmp";
	}

	std::vector<std::string> fileNames = { base_img_path, cmp_img_path };

	// multiple reading of files
	std::vector<std::shared_ptr<Matrix<Color24b>>> data = extractor.readFiles(fileNames);

	std::shared_ptr<Matrix<Color24b>> res = data[0];
	std::shared_ptr<Matrix<Color24b>> resChange = data[1];

	am::configuration::ConfigurationReader reader;

	auto conf = reader.getConfigurationFromFile(conf_path);
	const size_t opt_threads = am::common::getOptimalThreadsCount(conf->ThreadsMultiplier);

	algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf, loggerPtr);

	algorithm::ImagePair pair(res, resChange);
	algorithm::DescObjects rects1 = detector.getObjectsRects(pair);

	for (auto &rect : rects1) {
		printf("row:%zd col:%zd    row:%zd col:%zd value:%zd\n",
			rect.getMinHeight(), rect.getLeft(), rect.getMaxHeight(),
			rect.getRight(), rect.getPixelsCount());
	}

	/// draw found objects in BMP image file
	am::extraction::BmpDrawer drawer(fileNames[1]);
	for (auto obj : rects1)
	{
		drawer.drawRectangle(obj.getLeft(), obj.getMinHeight(), obj.getRight(),
			obj.getMaxHeight());
	}
	drawer.save(std::string(out_img_path));
	return 0;
}