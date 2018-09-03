#pragma once
#include "ColorChannels.hpp"
#include "common/Constants.hpp"

namespace am {
	namespace common {
		namespace types {

			static void setUpChannelDiff(const uint8_t source, const uint8_t compared, uint8_t& diff, uint8_t& posisitvness, const uint8_t addition)
			{
				if (source > compared)
				{
					diff = source - compared;
					posisitvness += addition;
				}
				else
					diff = compared - source;
			}

			//simple check if value in diff positive(true), otherwise - negative(false)
			static bool isChannelDiffPositive(uint8_t diff, int position)
			{
				return ((diff) & (1 << (position)));
			}

			struct ColorChannelsDiff : public ColorChannels
			{
				uint8_t positives;
				// bitmask for representations +/- for channel diffs
				// 0(min) - no positives; 8(max) - all positives.
				// 1 bit - r channel positiveness
				// 2 bit - g channel positiveness
				// 3 bit - b channel positiveness
				// only 3 bits used, other 5 can be used for other info
				/// why ? - to reduce memory usage, in case of HQ images(QHD, 4K) it can be important
				bool isPositveR()
				{
					return isChannelDiffPositive(positives, R_BIT_POSITION);
				}
				bool isPositveG()
				{
					return isChannelDiffPositive(positives, G_BIT_POSITION);
				}
				bool isPositveB()
				{
					return isChannelDiffPositive(positives, B_BIT_POSITION);
				}
			};

		}
	}
}