#pragma once

#include "common/exceptions/AmException.hpp"

namespace am
{
	namespace common
	{
		namespace exceptions
		{
			class AllocationException : public AmException
			{
			public:
				AllocationException(std::string &msg) : AmException(msg) {}
			};
		}
	}
}