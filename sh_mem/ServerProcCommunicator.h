#pragma once
#include "ProcCommunicator.h"

class ServerProcCommunicator : public ProcCommunicator
{
public:
    ServerProcCommunicator(const std::string &shMemName);
    virtual ~ServerProcCommunicator();

    void send(const Message *msg);
    Message *receive();
};