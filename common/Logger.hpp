#pragma once
#include <stdarg.h>
#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <mutex>

namespace am {
	namespace common {

		/// todo: move immplementation to source file to make possible make a stub empty class for UT
		class Logger {

		private:
			std::mutex mtx;
			static const size_t s_MaxPrefixSize = 128;
			static const size_t s_MaxBufferSize = 1024;
			const char* fileName = "last_logging.log";
			const char* INFO_TAG = "<INFO> ";
			const char* WARN_TAG = "<WARNING> ";
			const char* ERROR_TAG = "<ERROR> ";

		public:

			Logger(){};

			~Logger() = default;

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
				///todo: refactor if needed, unfortunatelly milliseconds value require additional calls
				std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
				std::time_t now_c = std::chrono::system_clock::to_time_t(now);

				std::stringstream str;
				//todo: deprecated localtime, needed swithc to localtime_s - as compiler suggesting :)
				str << tag << std::put_time(std::localtime(&now_c), "%Y-%m-%d %X:") <<
					std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

				va_list args;
				va_start(args, format);
				char buffer[s_MaxBufferSize];
				vsnprintf(buffer, s_MaxBufferSize, format, args);
				va_end(args);

				str << " " << buffer << "\n";

				// thread safe access to file, raii lock
				std::unique_lock<std::mutex> lock(mtx);
				open(fileName);
				write(str.str());

				_fileStream.close();
			}
			/// todo check if path needed
			bool open(const std::string& fileName)
			{
				_fileStream.open(fileName.c_str(), std::ofstream::in | std::ofstream::app);
				if (!_fileStream.is_open()) {
					std::cerr << "Error: Failed to open file '" << fileName << "\n";
					return false;
				}

				return true;
			}

			void write(const std::string& buffer)
			{
				if (_fileStream) {
					_fileStream << buffer;
				}
				else {
					/// exception FileAccess ?
					std::cerr << "Error: File stream is not open for writing.\n";
				}
			}

			std::ofstream _fileStream;
		};

	}
}