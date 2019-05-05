#pragma once
#include "Color24b.hpp"
#include "common/Constants.hpp"
#include <bitset>

namespace am {
	namespace common {
		namespace types {

			static void setUpChannelDiff(const uint8_t source, const uint8_t compared, uint8_t& diff, std::bitset<3>& positives, const uint8_t chId)
			{
				if (source > compared)
				{
					diff = source - compared;
					positives[chId] = true;
				}
				else
					diff = compared - source;
			}

			struct Color24bDiff : public Color24b
			{
				// bits for representations +/- for channel diffs
				// + = true = positive  ;   - = false = negative
				// 1 bit - r channel positiveness
				// 2 bit - g channel positiveness
				// 3 bit - b channel positiveness
				std::bitset<3> positives;

				bool isPositveR()
				{
					return positives[0];
				}
				bool isPositveG()
				{
					return positives[1];
				}
				bool isPositveB()
				{
					return positives[2];
				}
			};

		}
	}
}