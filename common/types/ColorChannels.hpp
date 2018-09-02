#pragma once
#include <stdint.h>


namespace am {
	namespace common {
		namespace types {
			// representation of particular pixel colors from image
			struct ColorChannels
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
			};
		}
	}
}