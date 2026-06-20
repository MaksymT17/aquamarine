#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "AmApi.h"
#include "MockMultipleExtractor.h"
#include "MockObjectDetector.h"

using namespace am::common::types;
using namespace am::analyze::algorithm;
using namespace am::unit_tests;

using ::testing::_;
using ::testing::Return;

TEST(AmApiTest, compareImagesAndGetConfiguration)
{
    Configuration conf;
    conf.AffinityThreshold = 42;

    auto extractorMock = std::make_unique<MockMultipleExtractor>();
    auto detectorMock = std::make_unique<MockObjectDetector>();

    // Set up return values
    std::vector<Matrix<Color24b>> matrices;
    matrices.emplace_back(10, 10);
    matrices.emplace_back(10, 10);
    EXPECT_CALL(*extractorMock, readFiles(_))
        .WillOnce(Return(matrices));

    DescObjects fakeObjects;
    ObjectRectangle rect(1, 1);
    rect.addPixel(5, 5);
    fakeObjects.emplace(rect);

    EXPECT_CALL(*detectorMock, getObjectsRects(_))
        .WillOnce(Return(fakeObjects));

    am::AmApi api(conf, std::move(extractorMock), std::move(detectorMock));

    auto result = api.compare("img1.bmp", "img2.bmp");

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result.begin()->getLeft(), 1);
    
    // Check configuration
    const Configuration& retrievedConf = api.getConfiguration();
    EXPECT_EQ(retrievedConf.AffinityThreshold, 42);
}
