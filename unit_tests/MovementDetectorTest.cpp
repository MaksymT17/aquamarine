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

TEST(MovementDetectionTest, checkMovementOnEqualImages)
{
	am::extraction::BmpExtractor extractor;
	std::string base("inputs/fhd_3obj.BMP");
	std::string next("inputs/fhd_3obj.BMP");

	auto firstBmp = extractor.readFile(base);
	auto secondtBmp = extractor.readFile(next);
	using namespace am::analyze::algorithm;
	movement::ImagePairPtr pair(std::make_shared<am::analyze::algorithm::ImagePair>(firstBmp, secondtBmp));

	std::multiset<Object, comparators::Descending> m_objs, result;

	// example of tracking object. No real movement shall be found
	std::vector<Pixel> pixels{{5, 5}, {10, 10}, {20, 20}};
	Object obj(pixels);
	m_objs.emplace(obj);
	am::configuration::ConfigurationReader reader;
	auto conf = reader.getConfigurationFromFile("inputs/configuration.csv");
	std::shared_ptr<am::common::Logger> lPtr(std::make_shared<am::common::Logger>("ut_dummy_logger.log"));
	movement::MovementDetector detector(4, conf, lPtr);
	detector.setTrackingObjects(m_objs);
	auto movements = detector.analyze(pair, result);

	// IMages are the same, no moving objects, class shall recognize it as STEALTH - disappear
	EXPECT_FALSE(movements[0].mov[0]);
	EXPECT_FALSE(movements[0].mov[1]);
	EXPECT_FALSE(movements[0].mov[2]);
	EXPECT_FALSE(movements[0].mov[3]);
	EXPECT_TRUE(movements[0].mov[4]); // check if STEALTH bit is set
}
