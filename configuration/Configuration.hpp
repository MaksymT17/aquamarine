#pragma once
#include <cstdint>

namespace am {
	namespace configuration
	{
		class Configuration{
		public:
			size_t AffinityThreshold;
			size_t MinPixelsForObject;
			uint8_t PixelStep;
			double CalculationTimeLimit;
			size_t IdleTimeout;
		};

	}
}