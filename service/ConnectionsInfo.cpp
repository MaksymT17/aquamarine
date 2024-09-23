#include "ConnectionsInfo.h"

bool ConnectionsInfo::isRequestValid(const Message *message)
{
    std::cout << "isRequestValid" << message->id << message->type << std::endl;
    auto client_iterator = std::find_if(clients.begin(), clients.end(), [&](const ClientInfo &c)
                                        { return c.id == message->id; });
    if (client_iterator == clients.end())
    {
        std::cout << "isRequestValid 1" << std::endl;
        // add clinet with default configuration
        clients.push_back({message->id, State::UNKNOWN, {75, 10, 1, 50, 5, 10.0}});
        client_iterator = std::find_if(clients.begin(), clients.end(), [&](const ClientInfo &c)
                                       { return c.id == message->id; });
    }
    if (message->type == MessageType::HANDSHAKE || message->type == MessageType::DISCONNECT)
    {
        std::cout << "isRequestValid HANDSHAKE  OK" << std::endl;
        return true;
    }
    else if (message->type == MessageType::SET_CONFIG)
    {
        std::cout << "isRequestValid SET_CONFIG  " << std::endl;
        return client_iterator->state >= State::READY ? true : false;
    }
    else if (message->type == MessageType::COMPARE_REQUEST)
    {
        std::cout << "isRequestValid COMPARE_REQUEST  " << std::endl;
        if (client_iterator->state == State::UNKNOWN)
        {
            return false;
        }
        else if (client_iterator->state >= State::READY)
        {
            std::cerr << "Client was not properly configured. Use default configuration." << std::endl;
            return client_iterator->state == State::CONFIGURED ? true : false;
        }
    }
    std::cerr << "isRequestValid unknown state. If new Message type has been added - extend validation for it." <<client_iterator->state<< std::endl;
    return false;
}

std::vector<ConnectionsInfo::ClientInfo>::const_iterator ConnectionsInfo::processActionUpdate(const Message *msg)
{
    auto client_iterator = std::find_if(clients.begin(), clients.end(), [&](const ClientInfo &c)
                                        { return c.id == msg->id; });
    if (client_iterator == clients.end())
    {
        std::cout << "Unknown client id: " << msg->id << std::endl;
        return client_iterator;
    }

    if (msg->type == MessageType::HANDSHAKE)
    {
        client_iterator->state = State::READY;
    }
    else if (msg->type == MessageType::SET_CONFIG)
    {
        const MessageSetConfig *messageSetConfig = static_cast<const MessageSetConfig *>(msg);
        client_iterator->state = State::CONFIGURED;
        std::memcpy(&client_iterator->configuration, &messageSetConfig->configuration, sizeof(Configuration));
        // printf("set_conf id:%zu  {%d %f %d %d %zd %f}\n", msg->id, client_iterator->configuration.AffinityThreshold,
        // client_iterator->configuration.CalculationTimeLimit, client_iterator->configuration.IdleTimeout,
        // client_iterator->configuration.MinPixelsForObject, client_iterator->configuration.PixelStep, client_iterator->configuration.ThreadsMultiplier);
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