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

const std::string shared_memory_name{"/_shmem1107"};
std::unique_ptr<ServerProcCommunicator> slave;

void handleSignal(int signal)
{
	std::cout << "Received SIGTERM signal (" << signal << "). Cleaning up and exiting...\n";
	slave.reset();
	
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

	slave = std::make_unique<ServerProcCommunicator>(shared_memory_name);
	Configuration default_conf{75, 10, 1, 50, 5, 10.0};
	std::unique_ptr<am::AmApi> amApi = std::make_unique<am::AmApi>(default_conf);

	// confirm master connection by Handshake message, and then set configuration
	// such order defines expected message flow
	ConnectionsInfo connections;

	bool isRunning = true;
	while (isRunning)
	{
		Message *message = slave->receive();
		printf("received %d message\n", message->type);
		if (!connections.isRequestValid(message))
		{
			std::cout << "received UNEXPECTED_REQUEST req\n"
					  << message->type << std::endl;
			Message response{message->id, MessageType::UNEXPECTED_REQUEST};
			slave->send(&response);
			continue;
		}
		if (message->type == MessageType::HANDSHAKE)
		{
			std::cout << "received HANDSHAKE req\n";
			Message msg{message->id, MessageType::HANDSHAKE_OK};
			auto iter = connections.processActionUpdate(message);
			slave->send(&msg);
		}
		else if (message->type == MessageType::SET_CONFIG)
		{
			MessageSetConfig *messageConf = static_cast<MessageSetConfig *>(message);
			std::cout << "received SET_CONFIG req px:" << messageConf->configuration.MinPixelsForObject << std::endl;
			Message msg{message->id, MessageType::SET_CONFIG_OK};
			auto iter = connections.processActionUpdate(message);
			Configuration newConf{messageConf->configuration.AffinityThreshold, messageConf->configuration.MinPixelsForObject, messageConf->configuration.PixelStep, messageConf->configuration.CalculationTimeLimit, messageConf->configuration.IdleTimeout, messageConf->configuration.ThreadsMultiplier};

			amApi->setConfiguration(newConf);
			std::cout << "received SET_CONFIG OK \n";
			slave->send(&msg);
			std::cout << "received SET_CONFIG OK sent\n";
		}
		else if (message->type == MessageType::COMPARE_REQUEST)
		{
			std::cout << "received COMPARE_REQUEST req\n";
			auto iter = connections.processActionUpdate(message);
			amApi->setConfiguration(iter->configuration);

			MessageCompareRequest *messageCompare = static_cast<MessageCompareRequest *>(message);
			if (messageCompare)
			{
				printf("compare_ %s %s _\n", messageCompare->base, messageCompare->to_compare);
				auto iter = connections.processActionUpdate(message);
				amApi->setConfiguration(iter->configuration); // set configuration for this client
				printf("conf pixs %d\n", iter->configuration.MinPixelsForObject);
				am::analyze::algorithm::DescObjects result;
				try
				{
					result = amApi->compare(messageCompare->base, messageCompare->to_compare);
				}
				catch (const am::common::exceptions::AmException exc)
				{
					std::cout << "Exception has been caught: " << exc.what() << ::std::endl;
					Message failed{messageCompare->id, MessageType::COMPARE_FAIL};
					slave->send(&failed);
					continue;
				}
				std::cout << "received after compare\n";
				MessageCompareResult compare_result;
				compare_result.id = messageCompare->id;

				compare_result.payload_bytes = result.size() * sizeof(Rect);
				Rect *rects = static_cast<Rect *>(compare_result.payload);
				size_t id = 0;
				for (auto &rectangle : result)
				{
					if (id == 100) // limit by 100 objects
					{
						compare_result.payload_bytes = 100 * sizeof(Rect);
						break;
					}
					printf("_ %zd %zd %zd %zd _\n", rectangle.getLeft(), rectangle.getRight(), rectangle.getMaxHeight(), rectangle.getMinHeight());
					rects[id++] = {rectangle.getLeft(), rectangle.getRight(), rectangle.getMaxHeight(), rectangle.getMinHeight()};
					printf("_send %zd %zd %zd %zd _\n", rects[id - 1].l, rects[id - 1].r, rects[id - 1].t, rects[id - 1].b);
				}
				compare_result.type = MessageType::COMPARE_RESULT;
				slave->send(&compare_result);
			}
		}
		else if (message->type == MessageType::DISCONNECT)
		{
			std::cout << "received DISCONNECT req\n";
			isRunning = false;
			Message msg{message->id, MessageType::DISCONNECT};
			auto iter = connections.processActionUpdate(message);

			slave->send(&msg);
		}
	}

	std::cout << "Aquamarine service performed all actions. Disconnect requested, exiting process..." << std::endl;

	return 0;
}