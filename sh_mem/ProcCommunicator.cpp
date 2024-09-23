#include "ProcCommunicator.h"
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#endif

ProcCommunicator::ProcCommunicator(const std::string &shMemName) : m_master_mem_name(shMemName + "_master"),
                                                                   m_slave_mem_name(shMemName + "_slave"),
                                                                   m_master_received_s(shMemName + "_m_rsem"),
                                                                   m_slave_received_s(shMemName + "_s_rsem"),
                                                                   m_master_sent_s(shMemName + "_m_sent"),
                                                                   m_slave_sent_s(shMemName + "_s_sent"),
                                                                   m_slave_ready_s(shMemName + "_s_ready")
{
}
ProcCommunicator::~ProcCommunicator()
{
#ifndef _WIN32
    if (sem_close(m_master_received) == -1)
    {
        perror("Failed to destroy m_master_received semaphore");
    }

    if (sem_close(m_slave_received) == -1)
    {
        perror("Failed to destroy m_slave_received semaphore");
    }

    if (sem_close(m_master_sent) == -1)
    {
        perror("Failed to destroy m_master_sent semaphore");
    }

    if (sem_close(m_slave_sent) == -1)
    {
        perror("Failed to destroy m_slave_sent semaphore");
    }

    if (sem_close(m_slave_ready) == -1)
    {
        perror("Failed to destroy m_slave_ready semaphore");
    }

   

#else
    if (m_master_received && !CloseHandle(m_master_received))
    {
        perror("Failed to destroy m_master_received semaphore");
    }
    if (m_slave_received && !CloseHandle(m_slave_received))
    {
        perror("Failed to destroy m_slave_received semaphore");
    }
    if (m_master_sent && !CloseHandle(m_master_sent))
    {
        perror("Failed to destroy m_master_sent semaphore");
    }
    if (m_slave_sent && !CloseHandle(m_slave_sent))
    {
        perror("Failed to destroy m_slave_sent semaphore");
    }
    if (m_slave_ready && !CloseHandle(m_slave_ready))
    {
        perror("Failed to destroy m_slave_ready semaphore");
    }
#endif
}
