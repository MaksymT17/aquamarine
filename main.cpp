#include <memory>
#include <stdio.h>

#include "analyze/algorithm/ObjectDetector.h"
#include "AmApi.h"

#include "sh_mem/ProcCommunicator.h"

int main(int argc, char *argv[])
{
	const std::string shared_memory_name{"/shared_mem"};
	bool isStopRequested{false}, connectionConfirmed{false};
	std::unique_ptr<ProcCommunicator> slave = std::make_unique<ProcCommunicator>(false, false,shared_memory_name);
	std::unique_ptr<am::AmApi> amApi;
	// confirm master connection by Handshake message, and then set configuration
	// such order defines expected message flow

	// confirm handshake, entry point
	std::cout << "main establish connection ...\n";
	// confirm set configuration
	Message *message = slave->receive();
	if (message->type == MessageType::HANDSHAKE)
	{
		std::cout << "received handshake req\n";
		Message msg{message->id + 1, MessageType::HANDSHAKE_OK};
		slave->send(&msg);
	}
	else
	{
		std::cout << "Connection failure, didn't receive Handshake msg. type:" << message->type << std::endl;
		exit(1);
	}

	while (!connectionConfirmed || !isStopRequested)
	{
		Message *message = slave->receive();
		if (message->type == MessageType::SET_CONFIG)
		{
			std::cout << "received set_config req\n";
			MessageSetConfig *messageSetConfig = static_cast<MessageSetConfig *>(message);
			if (messageSetConfig)
			{
				am::configuration::Configuration c;
				c.AffinityThreshold = messageSetConfig->configuration.AffinityThreshold;
				c.CalculationTimeLimit = messageSetConfig->configuration.CalculationTimeLimit;
				c.IdleTimeout = messageSetConfig->configuration.IdleTimeout;
				c.MinPixelsForObject = messageSetConfig->configuration.MinPixelsForObject;
				c.PixelStep = messageSetConfig->configuration.PixelStep;
				c.ThreadsMultiplier = messageSetConfig->configuration.ThreadsMultiplier;

				amApi = std::make_unique<am::AmApi>(c);

				Message msg{message->id + 1, MessageType::SET_CONFIG_OK};
				slave->send(&msg);
				connectionConfirmed = true;
				std::cout << "connectionConfirmed conf_ok\n"
						  << std::endl;
				break;
			}
		}
		else if (message->type == MessageType::DISCONNECT)
		{
			std::cout << "received disconnect req\n";
			Message msg_disconnect{message->id + 1, MessageType::DISCONNECT_OK};
			slave->send(&msg_disconnect);
			isStopRequested = true;
		}
		else
		{
			std::cout << "Connection failure, didn't receive Handshake msg. type:" << message->type << std::endl;
			exit(1);
		}
	}
	// main function messaging. expected: set_conf, compare_req, disconnect
	while (!isStopRequested)
	{
		Message *message = slave->receive();
		if (message->type == MessageType::SET_CONFIG)
		{
			std::cout << "received set_config req\n";
			MessageSetConfig *messageSetConfig = static_cast<MessageSetConfig *>(message);
			if (messageSetConfig)
			{
				am::configuration::Configuration c;
				c.AffinityThreshold = messageSetConfig->configuration.AffinityThreshold;
				c.CalculationTimeLimit = messageSetConfig->configuration.CalculationTimeLimit;
				c.IdleTimeout = messageSetConfig->configuration.IdleTimeout;
				c.MinPixelsForObject = messageSetConfig->configuration.MinPixelsForObject;
				c.PixelStep = messageSetConfig->configuration.PixelStep;
				c.ThreadsMultiplier = messageSetConfig->configuration.ThreadsMultiplier;

				amApi = std::make_unique<am::AmApi>(c);

				Message msg{message->id + 1, MessageType::SET_CONFIG_OK};
				slave->send(&msg);
				connectionConfirmed = true;
				std::cout << "connectionConfirmed config updated successfully\n"
						  << std::endl;
			}
		}
		else if (message->type == MessageType::COMPARE_REQUEST)
		{
			std::cout << "received compare req req\n";
			MessageCompareRequest *messageCompare = static_cast<MessageCompareRequest *>(message);
			if (messageCompare)
			{
				std::cout << "received before compare\n"
						  << messageCompare->base << std::endl;
				const char *b = messageCompare->base;
				const char *c = messageCompare->to_compare;
				printf("_ %s %s _\n", b, c);
				auto result = amApi->compare(b, c);
				std::cout << "received after compare\n";
				MessageCompareResult compare_result;
				// if (result.size() < 100)
				//{
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
				//}
				// else
				//{
				//	compare_result.type = MessageType::COMPARE_FAIL;
				//}

				slave->send(&compare_result);
			}
		}
		else if (message->type == MessageType::DISCONNECT)
		{
			std::cout << "received disconnect req\n";
			Message msg_disconnect{message->id + 1, MessageType::DISCONNECT_OK};
			slave->send(&msg_disconnect);
			isStopRequested = true;
		}
		else
		{
			std::cout << "Connection failure, didn't receive Handshake msg. type:" << message->type << std::endl;
			exit(1);
		}
	}

	std::cout << "Aquamarine service performed all actions. Disconnect requested, exiting process..." << std::endl;

	return 0;
}