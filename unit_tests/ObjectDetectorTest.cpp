#include "TestBase.hpp"

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

namespace am {
	namespace unit_tests {
		void testObjectDetectionResuts()
		{
			using namespace am::common::types;
			using namespace am::analyze;

			std::shared_ptr<am::common::Logger> loggerPtr(new am::common::Logger("od_dummy_log.log"));
			am::extraction::MultipleBmpExtractor extractor(loggerPtr);
			std::string base("inputs/fhd_clean.BMP");
			std::string toCompare("inputs/fhd_3obj.BMP");
			std::vector<std::string> fileNames = { base, toCompare };

			//multiple reading of files
			std::vector<std::shared_ptr<Matrix<Color24b>>> data = extractor.readFiles(fileNames);

			std::shared_ptr<Matrix<Color24b>> res = data[0];
			std::shared_ptr<Matrix<Color24b>> resChange = data[1];

			algorithm::ImagePair pair(res, resChange);

			const size_t opt_threads =  am::common::getOptimalThreadsCount();

			am::configuration::ConfigurationReader reader;

			auto conf = reader.getConfigurationFromFile("inputs/configuration.csv");

			//experimental - but, main feature currently, external review needed
			algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf, loggerPtr);
			algorithm::DiffObjectDetector diffDetector = algorithm::DiffObjectDetector(opt_threads, conf, loggerPtr);

			algorithm::DescObjects rects1 = detector.getObjectsRects(pair);
			algorithm::DescObjects rects2 = detector.getObjectsRects(pair);
			EXPECT_EQ(rects1.size(), rects2.size());
		}
	}
}

int main()
{
	am::unit_tests::testObjectDetectionResuts();
	am::unit_tests::PRINTF_RESULTS("ObjectDetection");
	return 0;
}