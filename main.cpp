#include <memory>
#include <stdio.h>
#include <vector>
#include <csignal>
#include <algorithm>

#include "analyze/algorithm/ObjectDetector.h"
#include "AmApi.h"
#include "ServerProcCommunicator.h"
#include "ClientProcCommunicator.h"
#include "service/ConnectionsInfo.h"
#include "service/SilberService.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>

std::unique_ptr<am::service::SilberService> server;

void handleSignal(int signal)
{
	std::cout << "Received SIGTERM signal (" << signal << "). Cleaning up and exiting...\n";
	if (server)
	{
		server->stop();
		server.reset();
	}
	exit(0);
}

void initLogging()
{
	try
	{
		// Create rotating logger: 8 files, 2 MB each
		auto logger = spdlog::rotating_logger_mt(
			"am",	   // logger name
			"logs/am.log", // log file path
			2 * 1024 * 1024,   // max file size: 2 MB
			8				   // max 8 rotated files
		);

		// Flush immediately on error level
		logger->flush_on(spdlog::level::err);

		spdlog::set_default_logger(logger);

		spdlog::info("SpdLogger has been configured.");
	}
	catch (const spdlog::spdlog_ex &ex)
	{
		std::cerr << "Log initialization failed: " << ex.what() << std::endl;
	}
}

int main(int argc, char *argv[])
{
	initLogging();
	spdlog::info("AM starting ...");
#ifndef _WIN32
	struct sigaction sa;
	sa.sa_handler = handleSignal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	// Set up handlers for SIGTERM and SIGINT
	if (sigaction(SIGTERM, &sa, nullptr) == -1)
	{
		std::cerr << "Error setting up SIGTERM handler\n";
		return 1;
	}
	if (sigaction(SIGINT, &sa, nullptr) == -1)
	{
		std::cerr << "Error setting up SIGINT handler\n";
		return 1;
	}
#endif

	const std::string shared_memory_name{"/_shmem1107"};
	server = std::make_unique<am::service::SilberService>(shared_memory_name);
	server->start();
	spdlog::info("Aquamarine service performed all actions. Disconnect requested, exiting process...");
	return 0;
}