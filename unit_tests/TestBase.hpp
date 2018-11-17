#pragma once

#include <functional>
#include "common/exceptions/AmException.hpp"


namespace am {
	namespace unit_tests {

		struct TestCounter {
			uint32_t total;
			uint32_t failed;
		};



#define EXPECT_THROW(statement, expected_exception, result) \
++result.total; \
try { \
statement; \
 } \
catch(expected_exception exc) \
{ \
printf("expect excception [%s]. Test OK.\n", exc.what()); \
} \
catch(...) \
{ \
result.failed++; \
printf("Unexpected exception thrown. Test FAILED.\n"); \
} \

#define EXPECT_NON_THROW(statement, result) \
++result.total; \
try { \
statement; \
 } \
catch(...) \
{ \
result.failed++; \
printf("Unexpected exception thrown. Test FAILED\n"); \
} \

		class TestBase {
		public:
			TestBase(TestCounter& result) :
				mResult(result) {}
			~TestBase() = default;

			void EXPECT_EQ(bool first, bool second)
			{
				mResult.total++;
				if (first != second) {
					mResult.failed++;
					printf("UT Failed, EXPECT TRUE not accepted. \n");
				}
			}

			void EXPECT_NOT_EQ(bool first, bool second)
			{
				mResult.total++;
				if (first == second) {
					mResult.failed++;
					printf("UT Failed, EXPECT FALSE not accepted. \n");
				}
			}

			virtual void doTest() = 0;
		protected:
			TestCounter & mResult;
		};
	}
}
