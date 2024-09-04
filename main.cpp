#include <memory>
#include <stdio.h>
#include <vector>
#include <csignal>
#include <algorithm>

#include "analyze/algorithm/ObjectDetector.h"
#include "AmApi.h"
#include "sh_mem/ServerProcCommunicator.h"
#include "sh_mem/ClientProcCommunicator.h"
#include "service/ConnectionsInfo.h"
#include "service/SilberService.h"

std::unique_ptr<am::service::SilberService> server;

void handleSignal(int signal)
{
	std::cout << "Received SIGTERM signal (" << signal << "). Cleaning up and exiting...\n";
	if (server)
	{
		server->stop();
		server.reset();
	}
	exit(0); // Exit the program with status code 0
}

int main(int argc, char *argv[])
{
#ifndef _WIN32
	struct sigaction sa;
	sa.sa_handler = handleSignal; // Set the handler function
	sa.sa_flags = 0;			  // No special flags
	sigemptyset(&sa.sa_mask);	  // No additional signals blocked during handler execution

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
	std::cout << "Aquamarine service performed all actions. Disconnect requested, exiting process..." << std::endl;

	return 0;
}