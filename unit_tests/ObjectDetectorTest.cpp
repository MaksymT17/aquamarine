
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
using namespace am::analyze::algorithm;
struct ObjectDetectorWrapper : public ::testing::Test
{
	ObjectDetectorWrapper() = default;

	~ObjectDetectorWrapper() override
	{
	}

	void SetUp() override
	{
		loggerPtr = std::make_shared<am::common::Logger>("od_dummy_log.log");
		extractor = std::make_unique<am::extraction::MultipleBmpExtractor>(loggerPtr);
	}

	void TearDown() override
	{
	}

	std::shared_ptr<am::common::Logger> loggerPtr;
	std::unique_ptr<am::extraction::MultipleBmpExtractor> extractor;
	const size_t opt_threads = am::common::getOptimalThreadsCount();
	am::configuration::ConfigurationReader reader;
	std::shared_ptr<Configuration> conf = reader.getConfigurationFromFile("inputs/configuration.csv");
};

TEST_F(ObjectDetectorWrapper, Check3ObjsFHD)
{
	std::string base("inputs/fhd_clean.BMP");
	std::string toCompare("inputs/fhd_3obj.BMP");
	std::vector<Matrix<Color24b>> data = extractor->readFiles({base, toCompare});

	algorithm::ImagePair pair(data[0], data[1]);
	algorithm::DiffObjectDetector diffDetector = algorithm::DiffObjectDetector(opt_threads, conf, loggerPtr);

	algorithm::DescObjects rects2 = diffDetector.getObjectsRects(pair);

	EXPECT_TRUE(rects2.size() >= 3);
}

TEST_F(ObjectDetectorWrapper, Check2Objs10x10)
{
	std::string base("inputs/10x10_clean.BMP");
	std::string toCompare("inputs/10x10_2obj.BMP");
	std::vector<Matrix<Color24b>> data = extractor->readFiles({base, toCompare});

	algorithm::ImagePair pair(data[0], data[1]);
	// configuration where object with 1 pixel could be detected
	Configuration conf_s10x10{1, 1, 1, 1, 1, 1};
	std::shared_ptr<Configuration> conf1 = std::make_shared<Configuration>(conf_s10x10);
	algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf1, loggerPtr);

	algorithm::DescObjects rects1 = detector.getObjectsRects(pair);

	EXPECT_EQ(rects1.size(), 2);
}

TEST_F(ObjectDetectorWrapper, Check5Objs20x20)
{
	std::string base("inputs/20x20_clean.BMP");
	std::string toCompare("inputs/20x20_5objs.BMP");
	std::vector<Matrix<Color24b>> data = extractor->readFiles({base, toCompare});

	algorithm::ImagePair pair(data[0], data[1]);
	// configuration where object with 1 pixel could be detected
	Configuration conf_s10x10{1, 1, 1, 1, 1, 1};
	std::shared_ptr<Configuration> conf1 = std::make_shared<Configuration>(conf_s10x10);
	algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf1, loggerPtr);

	algorithm::DescObjects rects1 = detector.getObjectsRects(pair);

	EXPECT_EQ(rects1.size(), 5);
}

TEST_F(ObjectDetectorWrapper, CheckInvalidSize)
{
	std::string base("inputs/20x20_clean.BMP");
	std::string toCompare("inputs/10x10_clean.BMP");
	std::vector<Matrix<Color24b>> data = extractor->readFiles({base, toCompare});
	EXPECT_THROW(algorithm::ImagePair pair(data[0], data[1]), am::common::exceptions::AmException);
}

TEST_F(ObjectDetectorWrapper, CheckTimeLimitation)
{
	constexpr float duration_70ms = 0.07f;
	constexpr float duration_additional_ms = 0.03f;

	std::string base("inputs/rs_1.bmp");
	std::string toCompare("inputs/rs_2.bmp");
	std::vector<Matrix<Color24b>> data = extractor->readFiles({base, toCompare});

	algorithm::ImagePair pair(data[0], data[1]);
	// configuration where object with 1 pixel could be detected
	Configuration conf_s10x10{50, 3, 1, duration_70ms, 1, 1};
	std::shared_ptr<Configuration> conf1 = std::make_shared<Configuration>(conf_s10x10);

	// note: big count of threads also require longer time to sync
	algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf1, loggerPtr);

	auto startTime = std::chrono::steady_clock::now();
	algorithm::DescObjects rects1 = detector.getObjectsRects(pair);

	auto timeNow = std::chrono::steady_clock::now();
	std::chrono::duration<double> duration = timeNow - startTime;
	EXPECT_TRUE(duration.count() > duration_70ms);
}

TEST(ObjectTest, check3Pixels)
{
	std::vector<Pixel> pixels{{5, 5}, {10, 10}, {20, 20}};
	Object obj(pixels);

	EXPECT_EQ(obj.getLeft(), 5);
	EXPECT_EQ(obj.getRight(), 20);

	EXPECT_EQ(obj.getMinHeight(), 5);
	EXPECT_EQ(obj.getMaxHeight(), 20);
	EXPECT_EQ(obj.getPixelsCount(), 3);
}

TEST(ObjectTest, check10Pixels)
{
	std::vector<Pixel> pixels{{5, 5}, {10, 10}, {20, 20}, {6, 6}, {7, 7}, {11, 11}, {3, 3}, {12, 6}, {13, 7}, {11, 99}};
	Object obj(pixels);

	EXPECT_EQ(obj.getLeft(), 3);
	EXPECT_EQ(obj.getRight(), 99);

	EXPECT_EQ(obj.getMinHeight(), 3);
	EXPECT_EQ(obj.getMaxHeight(), 20);
	EXPECT_EQ(obj.getPixelsCount(), 10);
}

TEST(ObjectTest, check15Pixels)
{
	std::vector<Pixel> pixels{{5, 5}, {10, 10}, {20, 20}, {6, 6}, {7, 7}, {11, 11}, {3, 3}, {12, 6}, {13, 7}, {11, 99}, {2, 100}, {3, 33}, {5, 6}, {13, 17}, {14, 9}};
	Object obj(pixels);

	EXPECT_EQ(obj.getLeft(), 3);
	EXPECT_EQ(obj.getRight(), 100);

	EXPECT_EQ(obj.getMinHeight(), 2);
	EXPECT_EQ(obj.getMaxHeight(), 20);
	EXPECT_EQ(obj.getPixelsCount(), 15);
}

TEST(ObjectTest, checkIsMergable)
{
	std::vector<Pixel> pixels{{5, 5}};
	Object obj(pixels);

	std::vector<Pixel> new_pixels{{6, 5}};
	Object new_obj(new_pixels);

	EXPECT_TRUE(new_obj.isMeargableToVertical(obj));
	EXPECT_NO_THROW(new_obj.mergeIfPossibleVerticalToMe(obj));

	EXPECT_EQ(new_obj.getLeft(), 5);
	EXPECT_EQ(new_obj.getRight(), 5);
	EXPECT_EQ(new_obj.getMinHeight(), 5);
	EXPECT_EQ(new_obj.getMaxHeight(), 6);
	EXPECT_EQ(new_obj.getPixelsCount(), 2);

	obj.clearPixelsCount();
	EXPECT_EQ(obj.getPixelsCount(), 0);
}

TEST(ObjectTest, checkObjectRectangle_2px)
{
	ObjectRectangle obj_rect(11, 12);
	obj_rect.addPixel(13, 14);
	EXPECT_EQ(obj_rect.getLeft(), 12);
	EXPECT_EQ(obj_rect.getRight(), 14);
	EXPECT_EQ(obj_rect.getMinHeight(), 11);
	EXPECT_EQ(obj_rect.getMaxHeight(), 13);

	EXPECT_EQ(obj_rect.getPixelsCount(), 2);
}

TEST(ObjectTest, checkObjectRectangle_3px)
{
	ObjectRectangle obj_rect(11, 12);
	obj_rect.addPixel(13, 14);
	obj_rect.addPixel(6, 7);

	EXPECT_EQ(obj_rect.getLeft(), 7	);
	EXPECT_EQ(obj_rect.getRight(), 14);
	EXPECT_EQ(obj_rect.getMinHeight(), 6);
	EXPECT_EQ(obj_rect.getMaxHeight(), 13);

	EXPECT_EQ(obj_rect.getPixelsCount(), 3);
}
