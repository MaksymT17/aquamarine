#pragma once

#include <memory>
#include <iostream>
#include "SharedMemorySender.h"
#include "SharedMemoryReceiver.h"
#include "Message.hpp"
#ifndef _WIN32
#include <semaphore.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h> 
#else
#include <windows.h>
#endif

constexpr int SEMAPHORE_DISABLED = 0;
constexpr int SEMAPHORE_ENABLED = 1;

class ProcCommunicator
{
protected:
    ProcCommunicator(const std::string &shMemName);
    virtual ~ProcCommunicator();

protected:
    std::unique_ptr<SharedMemorySender> m_sender;
    std::unique_ptr<SharedMemoryReceiver> m_receiver;

    std::string m_master_received_s;
    std::string m_slave_received_s;
    std::string m_master_sent_s;
    std::string m_slave_sent_s;
    std::string m_slave_ready_s;

    const std::string m_master_mem_name;
    const std::string m_slave_mem_name;

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