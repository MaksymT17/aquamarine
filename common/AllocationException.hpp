#pragma once

#include "common/AmException.hpp"

namespace am {
	namespace common {
		class AllocationException : public AmException
		{
		public:
			AllocationException(std::string& msg) :
				AmException(msg) {}
		};
	}
}