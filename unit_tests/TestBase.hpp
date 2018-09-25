#pragma once

#include <functional>
#include "common/exceptions/AmException.hpp"


namespace am {
	namespace unit_tests {

#define EXPECT_THROW(statement, expected_exception) \
try { \
statement; \
 } \
catch(expected_exception exc) \
{ \
printf("expect excception [%s]. Test OK.\n", exc.what()); \
} \
catch(...) \
{ \
printf("Unexpected exception thrown. Test FAILED.\n"); \
} \


#define EXPECT_NON_THROW(statement) \
try { \
statement; \
 } \
catch(...) \
{ \
printf("Unexpected exception thrown. Test FAILED\n"); \
} \

		class TestBase {
		public:
			void EXPECT_TRUE(bool first, bool second)
			{
				if (first != second) {
					printf("UT Failed, EXPECT TRUE not accepted. \n");
				}
			}

			void EXPECT_FALSE(bool first, bool second)
			{
				if (first == second) {
					printf("UT Failed, EXPECT FALSE not accepted. \n");
				}
			}

			virtual void doTest() = 0;
		};
	}
}
