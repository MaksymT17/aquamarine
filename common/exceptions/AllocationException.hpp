#pragma once

#include "common/exceptions/AmException.hpp"

namespace am::common::exceptions
{
	class AllocationException : public AmException
	{
	public:
		AllocationException(std::string &msg) : AmException(msg) {}
	};
} // namespace am::common::exceptions
