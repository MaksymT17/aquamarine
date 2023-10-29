#include "AffinityComparer.h"
#include "common/Constants.hpp"
#include "common/exceptions/AllocationException.hpp"
#include <future>
#include "common/exceptions/AmException.hpp"

namespace am::analyze
{
	using namespace am::common::types;
	static void fillPixelLineWithDiffs(const Matrix<Color24b> &base, const Matrix<Color24b> &toCompare,
									   Matrix<Color24bDiff> &result, size_t rowId, size_t width)
	{

		for (size_t i = 0; i < width; ++i)
		{
			result(rowId, i) = Color24bDiff(base(rowId, i), toCompare(rowId, i));
		}
	}

	static bool validateComparingRequestedSize(size_t sourceW, size_t sourceH, size_t targetW, size_t targetH)
	{
		return (sourceW == targetW) && (sourceH == targetH);
	}

	AffinityComparer::AffinityComparer(Matrix<Color24b> &base)
		: mBase(base),
		  mMode(DataMode::KEEP_BASE_FRAME)
	{
	}

	Matrix<Color24bDiff> AffinityComparer::compare(const Matrix<Color24b> &first,
												   const Matrix<Color24b> &second, size_t threadsNum)
	{
		const size_t width = first.getWidth();
		const size_t height = first.getHeight();
		if (!validateComparingRequestedSize(width, height, second.getWidth(), second.getHeight()))
		{
			std::string msg("AffinityComparer size of images are different.");
			throw am::common::exceptions::AmException(msg);
		}

		Matrix<Color24bDiff> result(width, height);

		size_t threadsCount = threadsNum > height ? height : threadsNum;

		std::vector<std::future<void>> futures;

		for (size_t portion = 0; portion < height; portion += threadsCount)
		{
			for (size_t i = 0; i < threadsCount; ++i)
			{
				futures.push_back(std::async(std::launch::async, fillPixelLineWithDiffs, std::ref(first), std::ref(second), std::ref(result), portion + i, width));
			}
			for (auto &e : futures)
				e.get();

			futures.clear();
		}

		// final section in case if width not divided normally on threads count
		for (size_t lastLines = (height / threadsCount) * threadsCount; lastLines < height; ++lastLines)
			futures.push_back(std::async(fillPixelLineWithDiffs, std::ref(first), std::ref(second), std::ref(result), lastLines, width));

		return result;
	}
} // namespace am::analyze
