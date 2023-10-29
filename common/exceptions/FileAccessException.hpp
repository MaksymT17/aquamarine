#pragma once
#include "common/exceptions/AmException.hpp"

namespace am::common::exceptions
{
	class FileAccessException : public AmException
	{
	public:
		FileAccessException(std::string &msg) : AmException(msg) {}
	};
} // namespace am::common::exceptions
