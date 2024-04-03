#pragma once
#include "common/exceptions/AmException.hpp"

namespace am::common::exceptions
{
	class WrongFormatException : public AmException
	{
	public:
		WrongFormatException(std::string &msg) : AmException(msg) {}
	};
} // namespace am::common::exceptions
