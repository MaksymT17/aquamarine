#include"ExceptionError.hpp"

class FileAccessException : public std::exception
{
public:
	FileAccessException(std::string& msg) :
		std::exception(msg.c_str()) {}
};