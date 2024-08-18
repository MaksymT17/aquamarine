#pragma once

#include <memory>
#include "SharedMemorySender.h"
#include "SharedMemoryReceiver.h"
#include "Message.hpp"
#ifndef _WIN32
#include <semaphore.h>
#else
#include <windows.h>
#endif
class ProcCommunicator
{
public:
    ProcCommunicator(const bool isMasterMode, const bool isMultipleMasters, const std::string &shMemName);
    ~ProcCommunicator();

    void send(const Message *msg);
    Message *receive();
    Message *sendAndGetResponse(const Message *msg);

private:
    std::unique_ptr<SharedMemorySender> m_sender;
    std::unique_ptr<SharedMemoryReceiver> m_receiver;
    bool m_master_mode;
    bool m_multiple_master;
#ifndef _WIN32
    sem_t *m_master_received;
    sem_t *m_slave_received;
    sem_t *m_master_sent;
    sem_t *m_slave_sent;
    sem_t *m_slave_ready;
#else
    HANDLE m_master_received;
    HANDLE m_slave_received;
    HANDLE m_master_sent;
    HANDLE m_slave_sent;
    HANDLE m_slave_ready;
#endif
};