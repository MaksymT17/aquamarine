#pragma once
#include "common/AmException.hpp"

namespace am {
	namespace common {

		class FileAccessException : public AmException
		{
		public:
			FileAccessException(std::string& msg) :
				AmException(msg) {}
		};
	}
}