#include "gtest/gtest.h"
#include "extraction/JpgExtractor.h"
#include "common/exceptions/FileAccessException.hpp"

using namespace am::common::types;

TEST(JpgExtractorTest, checkForNotExistingFile)
{
    am::extraction::JpgExtractor extractor;
    std::string base("inputs/NotExisting.jpg");

    EXPECT_THROW(extractor.readFile(base), am::common::exceptions::FileAccessException);
}

TEST(JpgExtractorTest, check10x10)
{
    am::extraction::JpgExtractor extractor;
    Matrix<Color24b> result = extractor.readFile("inputs/10x10_clean.jpg");
    EXPECT_EQ(result.getWidth(), 10);
    EXPECT_EQ(result.getHeight(), 10);
}
