#pragma once
#include <stdint.h>
#include <vector>
#include <limits>
#include <cstdlib>

namespace common {

	const int R_POS_VAL = 1;
	const int G_POS_VAL = 2;
	const int B_POS_VAL = 4;

	const int R_BIT_POSITION = 0;
	const int G_BIT_POSITION = 1;
	const int B_BIT_POSITION = 2;

	const uint16_t CHANGE = std::numeric_limits<uint16_t>::max();

} // common