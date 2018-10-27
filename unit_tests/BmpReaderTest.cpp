#include "TestBase.hpp"
#include "analyze/ThresholdDiffChecker.h"
#include "extraction/BmpExtractor.h"
#include "common/exceptions/FileAccessException.hpp"
#include "BmpReaderTest.hpp"

namespace am {
	namespace unit_tests {

		void checkForNotExistingFile(TestCounter& result)
		{
			am::extraction::BmpExtractor extractor;
			std::string base("../inputs/NotExisting.BMP");

			EXPECT_THROW(extractor.readFile(base), am::common::exceptions::FileAccessException, result);
		}

		void checkForExistingFile(TestCounter& result)
		{
			am::extraction::BmpExtractor extractor;
			std::string base("../../inputs/rs_1.BMP");

			EXPECT_NON_THROW(extractor.readFile(base), result);
		}

		void BmpReaderTest::doTest()
		{
			checkForNotExistingFile(mResult);
			checkForExistingFile(mResult);
		}
	}
}