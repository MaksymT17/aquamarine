#include "gtest/gtest.h"
#include "analyze/algorithm/movement/MovementDetector.h"
#include "common/Logger.hpp"
#include "configuration/ConfigurationReader.hpp"
#include "extraction/BmpExtractor.h"

using namespace am::analyze::algorithm;

std::shared_ptr<am::common::Logger> lPtr(std::make_shared<am::common::Logger>("ut_dummy_logger.log"));

TEST(MovementDetectionTest, checkTwoObjectsMovements)
{
	am::extraction::BmpExtractor extractor;
	std::string base("inputs/rs_1.BMP");
	std::string next("inputs/rs_2.BMP");

	auto firstBmp = extractor.readFile(base);
	auto secondtBmp = extractor.readFile(next);
	using namespace am::analyze::algorithm;
	movement::ImagePairPtr pair(std::make_shared<am::analyze::algorithm::ImagePair>(firstBmp, secondtBmp));

	std::multiset<ObjectRectangle, comparators::Descending> m1;
	am::configuration::ConfigurationReader reader;
	auto conf = reader.getConfigurationFromFile("inputs/configuration.csv");
	movement::MovementDetector detector(3, conf, lPtr);

	EXPECT_THROW(detector.analyze(pair, m1), am::common::exceptions::AmException);
}

TEST(MovementDetectionTest, checkMovementLeft)
{
	ObjectRectangle center(5, 5);
	center.addPixel(5, 6);
	ObjectRectangle left(5, 5);
	left.addPixel(5, 4);

	auto movement_l = movement::MovementDetector::getMovementFromObjRects(center, left);

	EXPECT_TRUE(movement_l.mov[0]);
	EXPECT_FALSE(movement_l.mov[1]);
	EXPECT_FALSE(movement_l.mov[2]);
	EXPECT_FALSE(movement_l.mov[3]);
	EXPECT_FALSE(movement_l.mov[4]);
}

TEST(MovementDetectionTest, checkMovementRight)
{
	ObjectRectangle center(5, 5);
	center.addPixel(5, 4);
	ObjectRectangle right(5, 5);
	right.addPixel(5, 6);

	auto movement_l = movement::MovementDetector::getMovementFromObjRects(center, right);

	EXPECT_FALSE(movement_l.mov[0]);
	EXPECT_TRUE(movement_l.mov[1]);
	EXPECT_FALSE(movement_l.mov[2]);
	EXPECT_FALSE(movement_l.mov[3]);
	EXPECT_FALSE(movement_l.mov[4]);
}

TEST(MovementDetectionTest, checkMovementTop)
{
	ObjectRectangle center(5, 5);
	center.addPixel(6, 5);
	ObjectRectangle top(5, 5);
	top.addPixel(4, 5);

	auto movement_l = movement::MovementDetector::getMovementFromObjRects(center, top);

	EXPECT_FALSE(movement_l.mov[0]);
	EXPECT_FALSE(movement_l.mov[1]);
	EXPECT_TRUE(movement_l.mov[2]);
	EXPECT_FALSE(movement_l.mov[3]);
	EXPECT_FALSE(movement_l.mov[4]);
}

TEST(MovementDetectionTest, checkMovementBottom)
{
	ObjectRectangle center(5, 5);
	center.addPixel(4, 5);
	ObjectRectangle bottom(5, 5);
	bottom.addPixel(6, 5);

	auto movement_l = movement::MovementDetector::getMovementFromObjRects(center, bottom);

	EXPECT_FALSE(movement_l.mov[0]);
	EXPECT_FALSE(movement_l.mov[1]);
	EXPECT_FALSE(movement_l.mov[2]);
	EXPECT_TRUE(movement_l.mov[3]);
	EXPECT_FALSE(movement_l.mov[4]);
}

TEST(MovementDetectionTest, checkMovementStealth)
{
	ObjectRectangle center(5, 5);
	ObjectRectangle stealth(5, 5);

	auto movement_l = movement::MovementDetector::getMovementFromObjRects(center, stealth);

	EXPECT_FALSE(movement_l.mov[0]);
	EXPECT_FALSE(movement_l.mov[1]);
	EXPECT_FALSE(movement_l.mov[2]);
	EXPECT_FALSE(movement_l.mov[3]);
	EXPECT_TRUE(movement_l.mov[4]);
}

TEST(MovementDetectionTest, checkMovementOnEqualImages)
{
	am::extraction::BmpExtractor extractor;
	std::string base("inputs/fhd_3obj.BMP");
	std::string next("inputs/fhd_3obj.BMP");

	auto firstBmp = extractor.readFile(base);
	auto secondtBmp = extractor.readFile(next);

	movement::ImagePairPtr pair(std::make_shared<am::analyze::algorithm::ImagePair>(firstBmp, secondtBmp));

	std::multiset<ObjectRectangle, comparators::Descending> m_objs, result;

	// example of tracking object. No real movement shall be found
	ObjectRectangle obj_r(5, 5);
	obj_r.addPixel(10, 10);
	obj_r.addPixel(20, 20);

	m_objs.emplace(obj_r);
	am::configuration::ConfigurationReader reader;
	auto conf = reader.getConfigurationFromFile("inputs/configuration.csv");
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
