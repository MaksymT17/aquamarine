
#include "gtest/gtest.h"
#include "analyze/ThresholdDiffChecker.h"
#include "extraction/BmpExtractor.h"
#include "common/exceptions/FileAccessException.hpp"

using namespace am::common::types;

TEST(BmpReaderTest, checkForNotExistingFile)
{
    am::extraction::BmpExtractor extractor;
    std::string base("inputs/NotExisting.BMP");

    EXPECT_THROW(extractor.readFile(base), am::common::exceptions::FileAccessException);
}

TEST(BmpReaderTest, checkForExistingFile)
{
    am::extraction::BmpExtractor extractor;
    std::string base("inputs/rs_1.BMP");

    EXPECT_NO_THROW(extractor.readFile(base));
}

TEST(BmpExtractorTest, check10x10)
{
    // check one color bmp, each pixel has the same values for RGB channels
    am::extraction::BmpExtractor extractor;
    Matrix<Color24b> result = extractor.readFile("inputs/10x10_clean.BMP");
    EXPECT_EQ(result.getWidth(), 10);
    EXPECT_EQ(result.getHeight(), 10);
    EXPECT_EQ(result(1, 1), result(2, 2));
    EXPECT_EQ(result(1, 1), result(3, 2));
    EXPECT_EQ(result(1, 1), result(9, 9));
    EXPECT_EQ(result(1, 1), result(5, 6));
    EXPECT_EQ(result(2, 7), result(7, 2));
}

TEST(BmpExtractorTest, check20x20)
{
    am::extraction::BmpExtractor extractor;
    Matrix<Color24b> result = extractor.readFile("inputs/20x20_clean.BMP");
    EXPECT_EQ(result.getWidth(), 20);
    EXPECT_EQ(result.getHeight(), 20);
    EXPECT_EQ(result(2, 2), result(3, 2));
    EXPECT_EQ(result(7, 1), result(7, 2));
    EXPECT_EQ(result(1, 5), result(3, 9));
    EXPECT_EQ(result(2, 1), result(5, 6));
    EXPECT_EQ(result(2, 7), result(7, 2));
}
