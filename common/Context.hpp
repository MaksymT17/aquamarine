#pragma once
#include <thread>
#include "Logger.hpp"
#include "configuration/ConfigurationReader.hpp"

namespace am {

	namespace common {
		// Context with general info
		// singleton implementation
		class Context {
		private:
			Context()
				: mOptimalThreadsCount(calculateOptimalThreadsCount())
			{
				mLogger.logInfo("Aquamarine starting ...");
			}

			size_t calculateOptimalThreadsCount()
			{
				const float MAX_CPU_LOAD_PERCENT = 0.7f;
				// 70% of CPU threads for our stuff, others - for maintaining background proccess

				unsigned int MAX_THREADS = std::thread::hardware_concurrency();
				const float concurrentCallsCount = static_cast<float>(MAX_THREADS) * MAX_CPU_LOAD_PERCENT;
				size_t threadsCount = concurrentCallsCount < 1.0f ? 1 : static_cast<size_t>(concurrentCallsCount);

				return threadsCount;
			}
			size_t mOptimalThreadsCount;
			static Context *inst;
			Logger mLogger;

		public:
			am::configuration::Configuration getConfiguration()
			{
				am::configuration::ConfigurationReader reader;
				return *reader.getConfigurationFromFile("inputs/configuration.csv").begin(); // begin because only 1 config there(no multiresolution support - todo)
			}

			static void release()
			{
				if (inst != nullptr)
					delete inst;
			}

			~Context() 
			{
				mLogger.logInfo("Aquamarine finish jobs, cleaning resources.");
			}

			static Context *getInstance() {
				if (inst == nullptr)
					inst = new Context();

				return inst;
			}

			size_t getOpimalThreadsCount() {
				return mOptimalThreadsCount;
			}
		};
	}
}