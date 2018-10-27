#include "BmpReaderTest.hpp"
int main()
{
	using namespace am::unit_tests;
	TestCounter result;

	BmpReaderTest(result).doTest();

	printf("Failed tests: %d,  Tests count: %d.\n", result.failed, result.total);
	
	return 0;
}