#pragma once
#include <stdint.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // win32

namespace am
{
	namespace common
	{

		static void wait_msec(uint16_t milliseconds) noexcept
		{
#ifdef WIN32
			Sleep(milliseconds);
#else
			usleep(milliseconds * 1000);
#endif // win32
		}

		static std::string get_datetime()
		{
			std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
			std::time_t now_c = std::chrono::system_clock::to_time_t(now);

			std::stringstream str;
			str << std::put_time(std::localtime(&now_c), "%Y-%m-%d %X:")
				<< std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
			return str.str();
		}

	} // common
} // am