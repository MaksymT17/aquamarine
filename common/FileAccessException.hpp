#pragma once
#include<exception>

namespace am {
	namespace common {
		class FileAccessException : public std::runtime_error
		{
		public:
			FileAccessException(std::string& msg) :
				std::runtime_error(msg.c_str()) {}
		};
	}
}