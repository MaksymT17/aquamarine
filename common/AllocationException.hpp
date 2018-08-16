#pragma once
#include"ExceptionError.hpp"

namespace common {
	class AllocationException : public std::exception
	{
	public:
		AllocationException(std::string& msg) :
			std::exception(msg.c_str()) {}
	};
}