#pragma once

#include <memory>
#include "common/types/Matrix.hpp"
#include "common/types/Color24bDiff.hpp"

namespace am::analyze
{
	class IComparer
	{
	public:
		virtual ~IComparer() = default;
	};
} // namespace am::analyze
