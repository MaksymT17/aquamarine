#include "TestBase.hpp"
#include "analyze/ThresholdDiffChecker.h"
#include "extraction/BmpExtractor.h"
#include "common/exceptions/FileAccessException.hpp"

namespace am {
	namespace unit_tests {
		void checkForNotExistingFile()
		{
			am::extraction::BmpExtractor extractor;
			std::string base("inputs/NotExisting.BMP");

			EXPECT_THROW(extractor.readFile(base), am::common::exceptions::FileAccessException);
		}
	}
}