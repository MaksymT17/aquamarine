#pragma once
#include "Color24b.hpp"
#include "common/Constants.hpp"
#include <bitset>

namespace am
{
	namespace common
	{
		namespace types
		{

			static bool isChannelDiffPositive(uint8_t diff, int position)
			{
				return ((diff) & (1 << (position)));
			}

			static void setUpChannelDiff(const uint8_t source, const uint8_t compared, uint8_t &diff,
										 uint8_t &posisitvness, const uint8_t addition)
			{
				if (source > compared)
				{
					diff = source - compared;
					posisitvness += addition;
				}
				else
					diff = compared - source;
			}

			struct Color24bDiff : public Color24b
			{
				Color24bDiff() : positives(0u){};

				Color24bDiff(Color24b base, Color24b cmpr) : positives(0u)
				{
					setUpChannelDiff(base.r, cmpr.r, r, positives, R_POS_VAL);
					setUpChannelDiff(base.g, cmpr.g, g, positives, G_POS_VAL);
					setUpChannelDiff(base.b, cmpr.b, b, positives, B_POS_VAL);
				}

				bool isPositveR() const noexcept
				{
					return isChannelDiffPositive(positives, R_BIT_POSITION);
				}

				bool isPositveG() const noexcept
				{
					return isChannelDiffPositive(positives, G_BIT_POSITION);
				}

				bool isPositveB() const noexcept
				{
					return isChannelDiffPositive(positives, B_BIT_POSITION);
				}

			private:
				uint8_t positives;
			};

		}
	}
}