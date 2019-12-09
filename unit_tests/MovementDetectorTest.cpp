#include "analyze/algorithm/movement/MovementDetector.h"
#include "TestBase.hpp"
#include "common/Logger.hpp"
#include "configuration/ConfigurationReader.hpp"
#include "extraction/BmpExtractor.h"

namespace am {
	namespace unit_tests {

		void checkTwoObjectsMovements() {
			am::extraction::BmpExtractor extractor;
			std::string base("../../inputs/rs_1.BMP");
			std::string next("../../inputs/rs_2.BMP");

			auto firstBmp = extractor.readFile(base);
			auto secondtBmp = extractor.readFile(next);
			am::analyze::algorithm::movement::ImagePairPtr pair(
				new am::analyze::algorithm::ImagePair(firstBmp, secondtBmp));
			using namespace am::analyze::algorithm;

			std::multiset<Object, comparators::Descending> m1;
			am::configuration::ConfigurationReader reader;
			auto conf = reader.getConfigurationFromFile("../../inputs/configuration.csv");
			std::shared_ptr<am::common::Logger> lPtr(
				new am::common::Logger("ut_dummy_logger.log"));
			movement::MovementDetector detector(3, conf, lPtr);

			EXPECT_THROW(detector.analyze(pair, m1), am::common::exceptions::AmException,
				test_results);
		}
	} // namespace unit_tests
} // namespace am

int main() {
	am::unit_tests::checkTwoObjectsMovements();
	am::unit_tests::PRINTF_RESULTS("MovementDetection");
	return 0;
}
