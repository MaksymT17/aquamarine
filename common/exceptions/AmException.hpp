#pragma once

#include <stdexcept>

namespace am::common::exceptions
{
	class AmException : public std::runtime_error
	{
	public:
		AmException(std::string &msg) : std::runtime_error(msg) {}
	};
} // namespace am::common::exceptions
