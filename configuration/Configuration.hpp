#pragma once
#include <cstdint>
namespace am::configuration
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

} // namespace am::configuration
