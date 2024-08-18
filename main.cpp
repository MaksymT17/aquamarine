#include <memory>
#include <stdio.h>
#include <vector>

#include "analyze/algorithm/ObjectDetector.h"
#include "AmApi.h"

#include "sh_mem/ProcCommunicator.h"
#include <algorithm>
enum State : size_t
{
	UNKNOWN = 0,
	READY,
	CONFIGURED
};
struct ClientInfo
{
	size_t id;
	State state;
	am::configuration::Configuration configuration;
};
struct ConnectionsInfo
{
	bool isRequestValid(const Message *message)
	{
		std::cerr << "isRequestValid" << message->id << message->type << std::endl;
		auto client_iterator = std::find_if(clients.begin(), clients.end(), [&](const ClientInfo &c)
											{ return c.id == message->id; });
		if (client_iterator == clients.end())
		{
			std::cerr << "isRequestValid 1" << std::endl;
			// add clinet with default configuration
			clients.push_back({message->id, State::UNKNOWN, {75, 10, 1, 50, 5, 10.0}});
			client_iterator = std::find_if(clients.begin(), clients.end(), [&](const ClientInfo &c)
										   { return c.id == message->id; });
		}
		if (message->type == MessageType::HANDSHAKE || message->type == MessageType::DISCONNECT)
		{
			std::cerr << "isRequestValid h d  OK" << std::endl;
			return true;
		}
		else if (message->type == MessageType::SET_CONFIG)
		{
			std::cerr << "isRequestValid set c  " << std::endl;
			return client_iterator->state >= State::READY ? true : false;
		}
		else if (message->type == MessageType::COMPARE_REQUEST)
		{
			std::cerr << "isRequestValid COMPARE_REQUEST  " << std::endl;
			if (client_iterator->state == State::UNKNOWN)
				return false;
			else if (client_iterator->state == State::READY)
				std::cerr << "Client was not properly configured. Use default configuration." << std::endl;
			return client_iterator->state == State::CONFIGURED ? true : false;
		}
	}

	std::vector<ClientInfo>::const_iterator processActionUpdate(const Message *msg)
	{
		auto client_iterator = std::find_if(clients.begin(), clients.end(), [&](const ClientInfo &c)
											{ return c.id == msg->id; });
		if (client_iterator == clients.end())
		{
			std::cerr << "Unknown client" << std::endl;
			return client_iterator;
		}

		if (msg->type == MessageType::HANDSHAKE)
			client_iterator->state = State::READY;
		else if (msg->type == MessageType::SET_CONFIG)
		{
			const MessageSetConfig *messageSetConfig = static_cast<const MessageSetConfig *>(msg);
			client_iterator->state = State::CONFIGURED;

			client_iterator->configuration.AffinityThreshold = messageSetConfig->configuration.AffinityThreshold;
			client_iterator->configuration.CalculationTimeLimit = messageSetConfig->configuration.CalculationTimeLimit;
			client_iterator->configuration.IdleTimeout = messageSetConfig->configuration.IdleTimeout;
			client_iterator->configuration.MinPixelsForObject = messageSetConfig->configuration.MinPixelsForObject;
			client_iterator->configuration.PixelStep = messageSetConfig->configuration.PixelStep;
			client_iterator->configuration.ThreadsMultiplier = messageSetConfig->configuration.ThreadsMultiplier;
			printf("set_conf id:%zu  {%d %f %d %d %zd %f}\n", msg->id, client_iterator->configuration.AffinityThreshold, client_iterator->configuration.CalculationTimeLimit, client_iterator->configuration.IdleTimeout, client_iterator->configuration.MinPixelsForObject, client_iterator->configuration.PixelStep, client_iterator->configuration.ThreadsMultiplier);
		}
		else if (msg->type == MessageType::DISCONNECT)
		{
			client_iterator->state = State::UNKNOWN;
		}
		else
		{
			std::cout << "No updates" << std::endl;
		}
		return client_iterator;
	}

	std::unique_ptr<ProcCommunicator> commuicator;
	std::unique_ptr<am::AmApi> amApi;
	std::vector<ClientInfo> clients;
};

int main(int argc, char *argv[])
{
	const std::string shared_memory_name{"/_shmem4"};
	bool isStopRequested{false}, connectionConfirmed{false};
	std::unique_ptr<ProcCommunicator> slave = std::make_unique<ProcCommunicator>(false, true, shared_memory_name);
	am::configuration::Configuration default_conf{75, 10, 1, 50, 5, 10.0};
	std::unique_ptr<am::AmApi> amApi = std::make_unique<am::AmApi>(default_conf);

	// confirm master connection by Handshake message, and then set configuration
	// such order defines expected message flow
	ConnectionsInfo connections;
	// confirm handshake, entry point

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
			std::cout << "received handshake req\n";
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
			am::configuration::Configuration newConf{messageConf->configuration.AffinityThreshold, messageConf->configuration.MinPixelsForObject, messageConf->configuration.PixelStep, messageConf->configuration.CalculationTimeLimit, messageConf->configuration.IdleTimeout, messageConf->configuration.ThreadsMultiplier};

			amApi->setConfiguration(newConf);
			slave->send(&msg);
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