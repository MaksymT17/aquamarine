#pragma once

#include<exception>

namespace am {
	namespace common {

		class AmException : public std::runtime_error
		{
		public:
			AmException(std::string& msg) :
				std::runtime_error(msg.c_str()) {}
		};
	}
}