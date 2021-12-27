#pragma once
#include <cstdint>

namespace am
{
	namespace configuration
	{

		struct Configuration
		{
			size_t AffinityThreshold;
			size_t MinPixelsForObject;
			uint8_t PixelStep;
			double CalculationTimeLimit;
			size_t IdleTimeout;
			double ThreadsMultiplier;
		};

	} // namespace configuration
} // namespace am