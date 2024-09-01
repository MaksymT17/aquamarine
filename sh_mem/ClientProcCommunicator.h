#pragma once
#include "ServerProcCommunicator.h"

class ClientProcCommunicator : public ProcCommunicator
{
public:
    ClientProcCommunicator(const std::string &shMemName);

    virtual ~ClientProcCommunicator() = default;

#ifndef _WIN32
    template <typename Response>
    void sendRequestGetResponse(const Message *request, Response &reponse)
    {
        sem_wait(m_slave_ready);
        m_sender->sendMessage(request);
        sem_post(m_master_sent);
        sem_wait(m_slave_received);
        sem_wait(m_slave_sent);

        Response *repsonsePtr = static_cast<Response *>(m_receiver->receiveMessage());

        if (repsonsePtr)
            reponse = Response(*repsonsePtr);
        else
            std::cerr << "ClientProcCommunicator::sendRequestGetResponse response type is not expected\n";

        sem_post(m_master_received);
        sem_post(m_slave_ready);  // release slave for next requests
    }
#else
    template <typename Response>
    void sendRequestGetResponse(const Message *request, Response &reponse)
    {
WaitForSingleObject(m_slave_ready, INFINITE);
        // sem_wait(m_slave_ready);

        m_sender->sendMessage(request);
        ReleaseSemaphore(m_master_sent, 1, NULL);
        // sem_post(m_master_sent);
WaitForSingleObject(m_slave_received, INFINITE);
        // sem_wait(m_slave_received);
WaitForSingleObject(m_slave_sent, INFINITE);
        // sem_wait(m_slave_sent);

        Response *repsonsePtr = static_cast<Response *>(m_receiver->receiveMessage());

        if (repsonsePtr)
            reponse = Response(*repsonsePtr);
        else
            std::cerr << "ClientProcCommunicator::sendRequestGetResponse response type is not expected\n";

ReleaseSemaphore(m_master_received, 1, NULL);
        //sem_post(m_master_received);
        ReleaseSemaphore(m_slave_ready, 1, NULL);
        //sem_post(m_slave_ready); // release slave for next messages
    }
#endif
};
