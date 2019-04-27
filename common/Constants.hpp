#pragma once
#include <stdint.h>
#include <limits>
#include <iostream>
#include <thread>

namespace am {
	namespace common {

		const int R_POS_VAL = 1;
		const int G_POS_VAL = 2;
		const int B_POS_VAL = 4;

		const int R_BIT_POSITION = 0;
		const int G_BIT_POSITION = 1;
		const int B_BIT_POSITION = 2;

		const uint16_t CHANGE = (std::numeric_limits<uint16_t>::max)();

		static size_t getOptimalThreadsCount(double max_load_persent = 1.0f)
		{
			unsigned int MAX_THREADS = std::thread::hardware_concurrency();
			const float concurrentCallsCount = static_cast<float>(MAX_THREADS) * max_load_persent;
			size_t threadsCount = concurrentCallsCount < 1.0f ? 1 : static_cast<size_t>(concurrentCallsCount);

			return threadsCount;
		}

	} // common
} // am