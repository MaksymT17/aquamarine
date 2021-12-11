#pragma once

// mv: deprecated file. gtests has its own implementation
// such header could be used for basic tests without linking gtest

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
} \
catch(...) \
{ \
result.failed++; \
} \

#define EXPECT_NON_THROW(statement, result) \
++result.total; \
try { \
statement; \
 } \
catch(...) \
{ \
result.failed++; \
} \

		static TestCounter test_results;

		static void EXPECT_EQ(bool first, bool second)
		{
			test_results.total++;
			if (first != second) {
				test_results.failed++;
			}
		}

		static void EXPECT_NOT_EQ(bool first, bool second)
		{
			test_results.total++;
			if (first == second) {
				test_results.failed++;
			}
		}

		static void PRINTF_RESULTS(const char* testName)
		{
			printf("%s Test result Failed:%d Total:%d.\n", testName,test_results.failed, test_results.total);
			printf("%s test: %s\n", testName, (test_results.failed > 0) ? "FAILED" : "PASSED");
		}

	}
}
