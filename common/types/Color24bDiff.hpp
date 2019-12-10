#pragma once
#include "Color24b.hpp"
#include "common/Constants.hpp"
#include <bitset>

namespace am {
	namespace common {
		namespace types {
			static void setUpChannelDiff(const uint8_t source, const uint8_t compared, uint8_t& diff, 
				uint8_t& posisitvness, const uint8_t addition)
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

			struct Color24bDiff : public Color24b
			{
				uint8_t positives;

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