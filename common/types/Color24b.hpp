#pragma once
#include <stdint.h>

namespace am
{
	namespace common
	{
		namespace types
		{
			// representation of particular pixel colors from image
			struct Color24b
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;

				bool operator==(const Color24b &rhs) const
				{
					return (r == rhs.r && g == rhs.g && b == rhs.b);
				}
			};
		}
	}
}