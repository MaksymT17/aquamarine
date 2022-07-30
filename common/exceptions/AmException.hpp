#pragma once

#include <stdexcept>

namespace am
{
	namespace common
	{
		namespace exceptions
		{
			class AmException : public std::runtime_error
			{
			public:
				AmException(std::string &msg) : std::runtime_error(msg.c_str()) {}
			};
		}
	}
}