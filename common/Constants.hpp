#pragma once
#include <iostream>
#include <limits>
#include <stdint.h>
#include <thread>

namespace am::common
{
	constexpr int R_POS_VAL = 1;
	constexpr int G_POS_VAL = 2;
	constexpr int B_POS_VAL = 4;

	constexpr int R_BIT_POSITION = 0;
	constexpr int G_BIT_POSITION = 1;
	constexpr int B_BIT_POSITION = 2;

	constexpr uint16_t CHANGE = (std::numeric_limits<uint16_t>::max)();

	/// note: multplier of threads per core
	/// HW dependent constant, bigger value can make calculations faster
	/// Warning: too big value creates threads initialization & switching overhead
	/// According to tests(i7-7700hq) acceptable range is: from 1.0 up to 8.0
	static size_t getOptimalThreadsCount(double thrd_per_core = 1.0f)
	{
		unsigned int HW_THREADS = std::thread::hardware_concurrency();
		const double app_threads = static_cast<double>(HW_THREADS) * thrd_per_core;

		return app_threads < 1.0f ? 1u : static_cast<size_t>(app_threads);
	}

} //  namespace am::common
