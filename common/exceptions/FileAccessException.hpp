#pragma once
#include "common/exceptions/AmException.hpp"

namespace am {
	namespace common {
		namespace exceptions {
			class FileAccessException : public AmException
			{
			public:
				FileAccessException(std::string& msg) :
					AmException(msg) {}
			};
		}
	}
}