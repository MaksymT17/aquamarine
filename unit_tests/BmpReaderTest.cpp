
#include "analyze/ThresholdDiffChecker.h"
#include "extraction/BmpExtractor.h"
#include "common/exceptions/FileAccessException.hpp"

#include "gtest/gtest.h"

namespace am {
	namespace unit_tests {

		void checkForNotExistingFile()
		{
			am::extraction::BmpExtractor extractor;
			std::string base("../inputs/NotExisting.BMP");

			EXPECT_THROW(extractor.readFile(base), am::common::exceptions::FileAccessException);
		}

		void checkForExistingFile()
		{
			am::extraction::BmpExtractor extractor;
			std::string base("../../inputs/rs_1.BMP");

			EXPECT_NO_THROW(extractor.readFile(base));
		}

	}
}

int main()
{
	using namespace am::unit_tests;
	checkForNotExistingFile();
	checkForExistingFile();
	return 0;
}