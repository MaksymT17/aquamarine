#pragma once
#include <stdarg.h>
#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <mutex>
#include "Timers.hpp"

namespace am
{
	namespace common
	{

		class Logger
		{
		public:
			explicit Logger(const char *fileName) : mFileName(fileName){};

			~Logger() = default;

			template <typename... Args>
			void info(const char *format, Args... args)
			{
				log(INFO_TAG, format, args...);
			}

			template <typename... Args>
			void warn(const char *format, Args... args)
			{
				log(WARN_TAG, format, args...);
			}

			template <typename... Args>
			void error(const char *format, Args... args)
			{
				log(ERROR_TAG, format, args...);
			}

		private:
			void log(const char *tag, const char *format, ...)
			{
				//std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
				//std::time_t now_c = std::chrono::system_clock::to_time_t(now);

				std::stringstream str;
				// todo: deprecated localtime, needed swithc to localtime_s - as compiler suggesting :)
				str << tag << get_datetime();

				va_list args;
				va_start(args, format);
				char buffer[s_MaxBufferSize];
				vsnprintf(buffer, s_MaxBufferSize, format, args);
				va_end(args);

				str << " " << buffer << "\n";

				// thread safe access to file, raii lock
				std::unique_lock<std::mutex> lock(mMutex);
				open();
				write(str.str());

				mFileStream.close();
			}
			/// todo check if path needed
			bool open()
			{
				mFileStream.open(mFileName, std::ofstream::in | std::ofstream::app);
				if (!mFileStream.is_open())
				{
					std::cerr << "Error: Failed to open file '" << mFileName << "\n";
					return false;
				}

				return true;
			}

			void write(const std::string &buffer)
			{
				if (mFileStream)
				{
					mFileStream << buffer;
				}
				else
				{
					/// exception FileAccess ?
					std::cerr << "Error: File stream is not open for writing.\n";
				}
			}
			std::mutex mMutex;
			static const size_t s_MaxPrefixSize = 128;
			static const size_t s_MaxBufferSize = 1024;
			const char *INFO_TAG = "<INFO> ";
			const char *WARN_TAG = "<WARNING> ";
			const char *ERROR_TAG = "<ERROR> ";
			const char *mFileName;
			std::ofstream mFileStream;
		};

	}
}