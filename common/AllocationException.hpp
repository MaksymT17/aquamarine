#pragma once
#include<exception>

namespace common {
	class AllocationException : public std::runtime_error
	{
	public:
		AllocationException(std::string& msg) :
			std::runtime_error(msg.c_str()) {}
	};
}