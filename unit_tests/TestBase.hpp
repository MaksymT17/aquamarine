#pragma once

#include<functional>
#include "common/AmException.hpp"

namespace am {
	namespace unit_tests {

		template<class T>
		class TestBase {
		public:
			TestBase(const T& testingObject) :
				T(testingObject) {}

			void EXPECT_TRUE(bool first, bool second)
			{
				if (first != second){
					printf("UT Failed %d, EXPECT TRUE not accepted. \n");
				}
			}

			void EXPECT_FALSE(bool first, bool second)
			{
				if (first == second) {
					printf("UT Failed %d, EXPECT FALSE not accepted. \n");
				}
			}

			void EXPECT_THROW(common::AmException* exception, std::function func...)
			{
				//try
				//{}
				//catch{}
			}

		private:
			T;
		};

	}
}