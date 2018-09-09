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
printf("expect excception [%s]\n", exc.what()); \
} \
catch(...) \
{ \
printf("Unexpected exception thrown\n"); \
} \


#define EXPECT_NON_THROW(statement, expected_exception) \
try { \
statement; \
 } \
catch(...) \
{ \
printf("Unexpected exception thrown\n"); \
} \


		void EXPECT_TRUE(bool first, bool second)
		{
			if (first != second) {
				printf("UT Failed %d, EXPECT TRUE not accepted. \n");
			}
		}

		void EXPECT_FALSE(bool first, bool second)
		{
			if (first == second) {
				printf("UT Failed %d, EXPECT FALSE not accepted. \n");
			}
		}

	}
}
