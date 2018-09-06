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
			size_t CalculationTimeLimit;
			size_t IdleTimeout;
		};

	}
}