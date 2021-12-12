#include "gtest/gtest.h"
#include "analyze/algorithm/movement/MovementDetector.h"
#include "common/Logger.hpp"
#include "configuration/ConfigurationReader.hpp"
#include "extraction/BmpExtractor.h"

TEST(MovementDetectionTest, checkTwoObjectsMovements)
{
	am::extraction::BmpExtractor extractor;
	std::string base("inputs/rs_1.BMP");
	std::string next("inputs/rs_2.BMP");

	auto firstBmp = extractor.readFile(base);
	auto secondtBmp = extractor.readFile(next);
	using namespace am::analyze::algorithm;
	movement::ImagePairPtr pair(std::make_shared<am::analyze::algorithm::ImagePair>(firstBmp, secondtBmp));

	std::multiset<Object, comparators::Descending> m1;
	am::configuration::ConfigurationReader reader;
	auto conf = reader.getConfigurationFromFile("inputs/configuration.csv");
	std::shared_ptr<am::common::Logger> lPtr(std::make_shared<am::common::Logger>("ut_dummy_logger.log"));
	movement::MovementDetector detector(3, conf, lPtr);

	EXPECT_THROW(detector.analyze(pair, m1), am::common::exceptions::AmException);
}
