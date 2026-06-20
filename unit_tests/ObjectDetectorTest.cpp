
#include "gtest/gtest.h"
#include "analyze/AffinityComparer.h"
#include <stdio.h>
#include "extraction/MultipleExtractor.h"
#include "extraction/ExtractorFactory.h"
#include "extraction/BmpExtractor.h"
#ifndef WIN32
#include "extraction/JpgExtractor.h"
#endif
#include "analyze/ThresholdDiffChecker.h"
#include "analyze/algorithm/BfsObjectDetector.h"
#include "analyze/algorithm/DiffObjectDetector.h"
#include "analyze/algorithm/ObjectDetector.h"
#include "analyze/algorithm/movement/MovementDetector.h"
#include "analyze/algorithm/ImagePair.h"
#include "configuration/ConfigurationReader.hpp"
#include "common/Context.hpp"
#include "extraction/BmpExtractor.h"
#include "MockMultipleExtractor.h"
#include <gmock/gmock.h>

using namespace am::common::types;
using namespace am::analyze;
using namespace configuration;
using namespace am::analyze::algorithm;

struct ObjectDetectorWrapper : public ::testing::Test
{
	ObjectDetectorWrapper() = default;

	~ObjectDetectorWrapper() override
	{
	}

	void SetUp() override
	{
		extractor = std::make_unique<am::unit_tests::MockMultipleExtractor>();
	}

	void TearDown() override
	{
	}

    std::vector<Matrix<Color24b>> createMatrices(size_t width, size_t height, int num_objs, size_t obj_size = 4) {
        Matrix<Color24b> base(width, height);
        Matrix<Color24b> compare(width, height);
        Color24b color{0, 0, 0};
        Color24b colorDiff{255, 255, 255};
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                base(y, x) = color;
                compare(y, x) = color;
            }
        }
        int objects_placed = 0;
        size_t spacing = obj_size + 2;
        size_t start_y = std::min((size_t)10, height / 2);
        size_t start_x = std::min((size_t)10, width / 2);
        for (size_t y = start_y; y + obj_size <= height && objects_placed < num_objs; y += spacing) {
            for (size_t x = start_x; x + obj_size <= width && objects_placed < num_objs; x += spacing) {
                for(size_t dy = 0; dy < obj_size; ++dy) {
                    for(size_t dx = 0; dx < obj_size; ++dx) {
                        compare(y + dy, x + dx) = colorDiff;
                    }
                }
                objects_placed++;
            }
        }
        return {base, compare};
    }

	std::unique_ptr<am::unit_tests::MockMultipleExtractor> extractor;
	const size_t opt_threads = am::common::getOptimalThreadsCount();
	Configuration conf{75, 1, 1, 50, 5, 10.0};
};

TEST_F(ObjectDetectorWrapper, Check3ObjsFHD)
{
    EXPECT_CALL(*extractor, readFiles(::testing::_)).WillOnce(::testing::Return(createMatrices(1920, 1080, 3)));
	std::vector<Matrix<Color24b>> data = extractor->readFiles({"fake1", "fake2"});

	algorithm::ImagePair pair(data[0], data[1]);

	algorithm::DiffObjectDetector diffDetector = algorithm::DiffObjectDetector(opt_threads, conf);

	algorithm::DescObjects rects2 = diffDetector.getObjectsRects(pair);
	
	EXPECT_TRUE(rects2.size() >= 3);
}

TEST_F(ObjectDetectorWrapper, Check2Objs10x10)
{
    EXPECT_CALL(*extractor, readFiles(::testing::_)).WillOnce(::testing::Return(createMatrices(10, 10, 2, 1)));
	std::vector<Matrix<Color24b>> data = extractor->readFiles({"fake1", "fake2"});

	algorithm::ImagePair pair(data[0], data[1]);
	// configuration where object with 1 pixel could be detected
	Configuration conf_s10x10{1, 1, 1, 1, 1, 1};
	Configuration conf1 = Configuration(conf_s10x10);
	algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf1);

	algorithm::DescObjects rects1 = detector.getObjectsRects(pair);

	EXPECT_EQ(rects1.size(), 2);
}

TEST_F(ObjectDetectorWrapper, Check5Objs20x20)
{
    EXPECT_CALL(*extractor, readFiles(::testing::_)).WillOnce(::testing::Return(createMatrices(20, 20, 5, 1)));
	std::vector<Matrix<Color24b>> data = extractor->readFiles({"fake1", "fake2"});

	algorithm::ImagePair pair(data[0], data[1]);
	// configuration where object with 1 pixel could be detected
	Configuration conf_s10x10{1, 1, 1, 1, 1, 1};
	Configuration conf1 = Configuration(conf_s10x10);
	algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf1);

	algorithm::DescObjects rects1 = detector.getObjectsRects(pair);

	EXPECT_EQ(rects1.size(), 5);
}

TEST_F(ObjectDetectorWrapper, CheckInvalidSize)
{
    std::vector<Matrix<Color24b>> badData;
    badData.emplace_back(20, 20);
    badData.emplace_back(10, 10);
    EXPECT_CALL(*extractor, readFiles(::testing::_)).WillOnce(::testing::Return(badData));

	std::vector<Matrix<Color24b>> data = extractor->readFiles({"fake1", "fake2"});
	EXPECT_THROW(algorithm::ImagePair pair(data[0], data[1]), am::common::exceptions::AmException);
}

TEST_F(ObjectDetectorWrapper, CheckTimeLimitation)
{
	constexpr float duration_70ms = 0.07f;
	constexpr float duration_additional_ms = 0.03f;

    EXPECT_CALL(*extractor, readFiles(::testing::_)).WillOnce(::testing::Return(createMatrices(1920, 1080, 5)));
	std::vector<Matrix<Color24b>> data = extractor->readFiles({"fake1", "fake2"});

	algorithm::ImagePair pair(data[0], data[1]);
	// configuration where object with 1 pixel could be detected
	Configuration conf_s10x10{50, 3, 1, duration_70ms, 1, 1};
	Configuration conf1 = Configuration(conf_s10x10);

	// note: big count of threads also require longer time to sync
	algorithm::ObjectDetector detector = algorithm::ObjectDetector(opt_threads, conf1);

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
