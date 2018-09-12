#pragma once
#include <string>
#include <ctime>
#include <stdarg.h>
#include <fstream>
#include <iostream>

namespace am {
	namespace common {

		class Logger {

		private:

			static const size_t s_MaxPrefixSize = 128;
			static const size_t s_MaxBufferSize = 1024;
			const char* fileName = "last_logging.log";
			const char* INFO_TAG = "INFO";
			const char* WARN_TAG = "WARNING";
			const char* ERROR_TAG = "ERROR";

		public:

			Logger() {
				open(fileName);
			}

			~Logger() {
				_fileStream.close();
			}

			template<typename... Args>
			void logInfo(const char* format, Args... args)
			{
				log(INFO_TAG, format, args...);
			}

			template<typename... Args>
			void logWarn(const char* format, Args... args)
			{
				log(WARN_TAG, format, args...);
			}

			template<typename... Args>
			void logError(const char* format, Args... args)
			{
				log(ERROR_TAG, format, args...);
			}

		private:
			void log(const char* tag, const char* format, ...)
			{
				std::time_t time = std::time(nullptr);
				std::string timeString = std::ctime(&time);

				char prefixString[s_MaxPrefixSize];
				snprintf(prefixString, s_MaxPrefixSize, "<%s>: %s ", tag, timeString.substr(0, timeString.size() - 1).c_str());

				va_list args;
				va_start(args, format);
				char buffer[s_MaxBufferSize];
				vsnprintf(buffer, s_MaxBufferSize, format, args);
				va_end(args);

				std::string message;
				message += prefixString;
				message += buffer;
				message += "\n";
				write(message);

			}
			/// todo check if path needed
			bool open(/*const std::string& path, */const std::string& fileName)
			{
				_fileStream.open(/*path + */fileName, std::ofstream::app);
				if (!_fileStream.is_open()) {
					std::cerr << "FileLogPolicy Error: Failed to open file '" << fileName << "' at " << "\n";
					return false;
				}

				_fileStream << "-- File Log Started --\n\n";

				return true;
			}

			void write(const std::string& buffer)
			{
				if (_fileStream) {
					_fileStream << buffer;
				}
				else {
					std::cerr << "FileLogPolicy Error: File stream is not open for writing.\n";
				}
			}

			std::ofstream _fileStream;
		};

	}
}