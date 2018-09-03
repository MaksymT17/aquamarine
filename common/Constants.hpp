#pragma once
#include <stdint.h>
#include <limits>
#include <iostream>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // win32


namespace am {
	namespace common {

		const int R_POS_VAL = 1;
		const int G_POS_VAL = 2;
		const int B_POS_VAL = 4;

		const int R_BIT_POSITION = 0;
		const int G_BIT_POSITION = 1;
		const int B_BIT_POSITION = 2;

		const uint16_t CHANGE =  (std::numeric_limits<uint16_t>::max)();

		/* not used for for now
		void wait_msec(int milliseconds)
		{
#ifdef WIN32
			Sleep(milliseconds);
#else
			usleep(milliseconds * 1000);
#endif // win32
		}
		*/
	} // common
}