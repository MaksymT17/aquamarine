#pragma once
#include <stdint.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // win32

namespace am {
	namespace common {

		void wait_msec(uint16_t milliseconds) noexcept{
#ifdef WIN32
			Sleep(milliseconds);
#else
			usleep(milliseconds * 1000);
#endif // win32
		}

	} // common
} // am