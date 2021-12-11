
#include "gtest/gtest.h"
#include "analyze/ThresholdDiffChecker.h"
#include "extraction/BmpExtractor.h"
#include "common/exceptions/FileAccessException.hpp"

TEST(BmpReaderTest, checkForNotExistingFile)
{
	am::extraction::BmpExtractor extractor;
	std::string base("../inputs/NotExisting.BMP");

	EXPECT_THROW(extractor.readFile(base), am::common::exceptions::FileAccessException);
}

TEST(BmpReaderTest, checkForExistingFile)
{
	am::extraction::BmpExtractor extractor;
	std::string base("../../inputs/rs_1.BMP");

	EXPECT_NO_THROW(extractor.readFile(base));
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}