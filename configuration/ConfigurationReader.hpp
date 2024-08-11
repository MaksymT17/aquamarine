#pragma once

#include "Configuration.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include "common/exceptions/FileAccessException.hpp"
#include "common/exceptions/AmException.hpp"
#include "analyze/algorithm/ObjectDetector.h"

namespace
{
	const char PARAM_LIMITER = ':';
	size_t PARAM_COUNT = 6;
}

namespace am::configuration
{
	class ConfigurationReader
	{
	public:
		ConfigurationReader() {}
	
		Configuration getConfigurationFromFile(const char *filePath)
		{

			std::ifstream file(filePath, std::ifstream::in);

			if (!file.is_open())
			{
				std::string errorMsg("Configuration file access failed!");
				throw common::exceptions::FileAccessException(errorMsg);
			}

			while (file.good())
			{
				Configuration rec;
				getConfiguration(rec, file);
				return rec;
			}

			std::string errorMsg("Configuration parse failed, check file parameters!");
			throw common::exceptions::AmException(errorMsg);
		}

	private:
		void setParameter(std::istream &line, Configuration &conf)
		{
			std::string paramName, paramVal;
			std::getline(line, paramName, PARAM_LIMITER);

			std::getline(line, paramVal, PARAM_LIMITER);
			if (paramName.compare("Affinity_Threshold") == 0)
				conf.AffinityThreshold = std::atoi(paramVal.c_str());
			else if (paramName.compare("Minimum_Pixels_In_Object") == 0)
				conf.MinPixelsForObject = std::atoi(paramVal.c_str());
			else if (paramName.compare("Pixel_Step") == 0)
				conf.PixelStep = std::atoi(paramVal.c_str());
			else if (paramName.compare("Calculation_Time_Limit") == 0)
				conf.CalculationTimeLimit = std::atof(paramVal.c_str());
			else if (paramName.compare("Idle_Timeout") == 0)
				conf.IdleTimeout = std::atoi(paramVal.c_str());
			else if (paramName.compare("Threads_Multiplier") == 0)
				conf.ThreadsMultiplier = std::atoi(paramVal.c_str());
		}

		void getConfiguration(Configuration &rec, std::istream &str)
		{
			for (size_t i = 0; i < PARAM_COUNT; ++i)
			{
				std::string line;
				std::getline(str, line);

				std::stringstream lineStream(line);
				setParameter(lineStream, rec);
			}
		}
	};
} // namespace am::configuration
