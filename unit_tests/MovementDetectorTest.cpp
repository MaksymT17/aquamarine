#include "gtest/gtest.h"
#include "analyze/algorithm/movement/MovementDetector.h"
#include "configuration/ConfigurationReader.hpp"
#include "extraction/BmpExtractor.h"

using namespace am::analyze::algorithm;


TEST(MovementDetectionTest, checkTwoObjectsMovements)
{
	am::extraction::BmpExtractor extractor;
	std::string base("inputs/rs_1.bmp");
	std::string next("inputs/rs_2.bmp");

	auto firstBmp = extractor.readFile(base);
	auto secondtBmp = extractor.readFile(next);
	using namespace am::analyze::algorithm;
	movement::ImagePairPtr pair(std::make_shared<am::analyze::algorithm::ImagePair>(firstBmp, secondtBmp));

	std::multiset<ObjectRectangle, comparators::Descending> m1;
	configuration::ConfigurationReader reader;
	auto conf = reader.getConfigurationFromFile("inputs/configuration.csv");
	movement::MovementDetector detector(3, conf);

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
	std::string base("inputs/fhd_3obj.bmp");
	std::string next("inputs/fhd_3obj.bmp");

	auto firstBmp = extractor.readFile(base);
	auto secondtBmp = extractor.readFile(next);

	movement::ImagePairPtr pair(std::make_shared<am::analyze::algorithm::ImagePair>(firstBmp, secondtBmp));

	std::multiset<ObjectRectangle, comparators::Descending> m_objs, result;

	// example of tracking object. No real movement shall be found
	ObjectRectangle obj_r(5, 5);
	obj_r.addPixel(10, 10);
	obj_r.addPixel(20, 20);

	m_objs.emplace(obj_r);
	configuration::ConfigurationReader reader;
	auto conf = reader.getConfigurationFromFile("inputs/configuration.csv");
	movement::MovementDetector detector(4, conf);
	detector.setTrackingObjects(m_objs);
	auto movements = detector.analyze(pair, result);

	// IMages are the same, no moving objects, class shall recognize it as STEALTH - disappear
	EXPECT_FALSE(movements[0].mov[0]);
	EXPECT_FALSE(movements[0].mov[1]);
	EXPECT_FALSE(movements[0].mov[2]);
	EXPECT_FALSE(movements[0].mov[3]);
	EXPECT_TRUE(movements[0].mov[4]); // check if STEALTH bit is set
}

using namespace am::common::types;

TEST(MovementDetectionTest, emptyTrackingObjectsWarns)
{
	configuration::ConfigurationReader reader;
	auto conf = reader.getConfigurationFromFile("inputs/configuration.csv");
	movement::MovementDetector detector(3, conf);
	std::multiset<ObjectRectangle, comparators::Descending> empty_objs;
	detector.setTrackingObjects(empty_objs);
}

TEST(MovementDetectionTest, checkMovementWithActualDifferencesMultipleObjects)
{
	Matrix<Color24b> bmp1(20, 20);
	Matrix<Color24b> bmp2(20, 20);

	// Make the second image different in two separate spots within the tracking rectangle
	bmp2(5, 5).r = 255;
	bmp2(15, 15).r = 255;

	movement::ImagePairPtr pair(std::make_shared<am::analyze::algorithm::ImagePair>(bmp1, bmp2));

	std::multiset<ObjectRectangle, comparators::Descending> m_objs, result;

	// Track the whole image
	ObjectRectangle obj_r(0, 0);
	obj_r.addPixel(19, 19);
	m_objs.emplace(obj_r);

	configuration::ConfigurationReader reader;
	auto conf = reader.getConfigurationFromFile("inputs/configuration.csv");
	conf.AffinityThreshold = 100; // diff is 255
	movement::MovementDetector detector(1, conf);
	detector.setTrackingObjects(m_objs);

	auto movements = detector.analyze(pair, result);
	
	EXPECT_EQ(movements.size(), 1);
	EXPECT_EQ(result.size(), 2); // 2 objects found
}

TEST(MovementDetectionTest, checkMovementWithActualDifferencesSingleObject)
{
	Matrix<Color24b> bmp1(20, 20);
	Matrix<Color24b> bmp2(20, 20);

	// Make the second image different in one spot
	bmp2(5, 5).r = 255;

	movement::ImagePairPtr pair(std::make_shared<am::analyze::algorithm::ImagePair>(bmp1, bmp2));

	std::multiset<ObjectRectangle, comparators::Descending> m_objs, result;

	ObjectRectangle obj_r(0, 0);
	obj_r.addPixel(19, 19);
	m_objs.emplace(obj_r);

	configuration::ConfigurationReader reader;
	auto conf = reader.getConfigurationFromFile("inputs/configuration.csv");
	conf.AffinityThreshold = 100;
	movement::MovementDetector detector(1, conf);
	detector.setTrackingObjects(m_objs);

	auto movements = detector.analyze(pair, result);
	
	EXPECT_EQ(movements.size(), 1);
	EXPECT_EQ(result.size(), 1);
}
