#include "TestBase.hpp"

namespace am {
	namespace unit_tests {
		class BmpReaderTest : public TestBase
		{
		public:
			BmpReaderTest(TestCounter& result) :TestBase(result) {};
			~BmpReaderTest() = default;
			virtual void doTest() override;
		};
	}
}