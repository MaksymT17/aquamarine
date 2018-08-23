#pragma once
#include<stdint.h>
#include <vector>
#include <limits>

namespace common {

	const int R_POS_VAL = 1;
	const int G_POS_VAL = 2;
	const int B_POS_VAL = 4;

	const int R_BIT_POSITION = 0;
	const int G_BIT_POSITION = 1;
	const int B_BIT_POSITION = 2;

	const uint16_t CHANGE = std::numeric_limits<uint16_t>::max();

	//simple check if value in diff positive(true), otherwise - negative(false)
	static bool isChannelDiffPositive(uint8_t diff, int position)
	{
		return ((diff) & (1 << (position)));
	}
	struct ColorChannels
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		// representation of particular pixel colors from image
		// currently alpha cahnnel not supported, it will be possible for formats(PNG etc.)
	};

	struct ColorChannelsDiff : public ColorChannels
	{
		uint8_t positives;
		// bitmask for representations +/- for channel diffs
		// 0(min) - no positives; 8(max) - all positives.
		// 1 bit - r channel positivness
		// 2 bit - g channel positivness
		// 3 bit - b channel positivness
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

	template<class T>
	class Matrix
	{
	public:
		Matrix(size_t width, size_t height)
			: mWidth(width),
			mHeight(height),
			mData(width * height)
		{
		}

		size_t getWidth()const { return mWidth; }
		size_t getHeight()const { return mHeight; }

		T& operator()(size_t i, size_t j)
		{
			return mData[i * mWidth + j];
		}

		T operator()(size_t i, size_t j) const
		{
			return mData[i * mWidth + j];
		}

		void push_back(T elem)
		{
			mData.push_back(elem);
		}

	private:
		size_t mWidth;
		size_t mHeight;
		std::vector<T> mData;
	};

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
} // common
