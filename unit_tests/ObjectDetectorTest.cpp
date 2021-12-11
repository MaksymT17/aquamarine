
#include "gtest/gtest.h"
#include "analyze/AffinityComparer.h"
#include <stdio.h>
#include "extraction/MultipleBmpExtractor.h"
#include "extraction/BmpExtractor.h"
#include "analyze/ThresholdDiffChecker.h"
#include "analyze/algorithm/BfsObjectDetector.h"
#include "analyze/algorithm/DiffObjectDetector.h"
#include "analyze/algorithm/ObjectDetector.h"
#include "analyze/algorithm/movement/MovementDetector.h"
#include "analyze/algorithm/ImagePair.h"
#include "configuration/ConfigurationReader.hpp"
#include "common/Context.hpp"
#include <memory>

using namespace am::common::types;
using namespace am::analyze;
using namespace am::configuration;

struct ObjectDetectorWrapper : public ::testing::Test
{
    ObjectDetectorWrapper() = default;

    ~ObjectDetectorWrapper() override {
    }

    void SetUp() override {
		loggerPtr = std::make_shared<am::common::Logger>("od_dummy_log.log");
		extractor = std::make_unique<am::extraction::MultipleBmpExtractor>(loggerPtr);
    }

    void TearDown() override {
    }

	std::shared_ptr<am::common::Logger> loggerPtr;
	std::unique_ptr<am::extraction::MultipleBmpExtractor> extractor;
	const size_t opt_threads = am::common::getOptimalThreadsCount();
	am::configuration::ConfigurationReader reader;
	std::shared_ptr<Configuration> conf = reader.getConfigurationFromFile("inputs/configuration.csv");
};

TEST_F(ObjectDetectorWrapper, Check3ObjsFHD) {
	std::string base("inputs/fhd_clean.BMP");
	std::string toCompare("inputs/fhd_3obj.BMP");
	std::vector<std::string> fileNames = { base, toCompare };
	std::vector<Matrix<Color24b>> data = extractor->readFiles(fileNames);

	algorithm::ImagePair pair(data[0], data[1]);
	algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf, loggerPtr);
	algorithm::DiffObjectDetector diffDetector = algorithm::DiffObjectDetector(opt_threads, conf, loggerPtr);

	algorithm::DescObjects rects1 = detector.getObjectsRects(pair);
	algorithm::DescObjects rects2 = detector.getObjectsRects(pair);
	EXPECT_EQ(rects1.size(), rects2.size());
	// because of noise, there found more objects but ist ok, it tracks any dfference according to conf
	EXPECT_TRUE(rects1.size() == 15);
}

TEST_F(ObjectDetectorWrapper, Check2Objs10x10) {
	std::string base("inputs/10x10_clean.BMP");
	std::string toCompare("inputs/10x10_2obj.BMP");
	std::vector<std::string> fileNames = { base, toCompare };
	std::vector<Matrix<Color24b>> data = extractor->readFiles(fileNames);

	algorithm::ImagePair pair(data[0], data[1]);
	//configuration where object with 1 pixel could be detected
	Configuration conf_s10x10{1,1,1,1,1,1};
	std::shared_ptr<Configuration> conf1=std::make_shared<Configuration>(conf_s10x10);
	algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf1, loggerPtr);

	algorithm::DescObjects rects1 = detector.getObjectsRects(pair);
	EXPECT_EQ(rects1.size(), 2);
}

TEST_F(ObjectDetectorWrapper, Check5Objs20x20) {
	std::string base("inputs/20x20_clean.BMP");
	std::string toCompare("inputs/20x20_5objs.BMP");
	std::vector<std::string> fileNames = { base, toCompare };
	std::vector<Matrix<Color24b>> data = extractor->readFiles(fileNames);

	algorithm::ImagePair pair(data[0], data[1]);
	//configuration where object with 1 pixel could be detected
	Configuration conf_s10x10{1,1,1,1,1,1};
	std::shared_ptr<Configuration> conf1=std::make_shared<Configuration>(conf_s10x10);
	algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf1, loggerPtr);

	algorithm::DescObjects rects1 = detector.getObjectsRects(pair);

	EXPECT_EQ(rects1.size(), 5);
}


int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}