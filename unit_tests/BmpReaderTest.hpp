#include "TestBase.hpp"

namespace am {
	namespace unit_tests {
		class BmpReaderTest : public TestBase
		{
		public:
			BmpReaderTest() = default;
			~BmpReaderTest() = default;
			virtual void doTest() override;
		};
	}
}