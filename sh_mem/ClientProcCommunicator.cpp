#include "ClientProcCommunicator.h"

ClientProcCommunicator::ClientProcCommunicator(
    const std::string &shMemName) : ProcCommunicator(shMemName)
{
    m_sender = std::make_unique<SharedMemorySender>( m_master_mem_name.c_str());
    m_receiver = std::make_unique<SharedMemoryReceiver>(m_slave_mem_name.c_str());

#ifndef _WIN32
    m_master_received = sem_open(m_master_received_s.c_str(), O_CREAT, 0666, SEMAPHORE_DISABLED);
    m_slave_received = sem_open(m_slave_received_s.c_str(), O_CREAT, 0666, SEMAPHORE_DISABLED);
    m_master_sent = sem_open(m_master_sent_s.c_str(), O_CREAT, 0666, SEMAPHORE_DISABLED);
    m_slave_sent = sem_open(m_slave_sent_s.c_str(), O_CREAT, 0666, SEMAPHORE_DISABLED);
    m_slave_ready = sem_open(m_slave_ready_s.c_str(), O_CREAT, 0666, SEMAPHORE_ENABLED);

    if (m_master_received == SEM_FAILED || m_slave_received == SEM_FAILED ||
        m_master_sent == SEM_FAILED || m_slave_sent == SEM_FAILED || m_slave_ready == SEM_FAILED || m_slave_ready == SEM_FAILED)
    {
        perror("ProcCommunicator sem_open failure.");
        exit(1);
    }
#else
    std::wstring wshMemName(shMemName.begin(), shMemName.end());

    m_master_received = CreateSemaphoreW(NULL, SEMAPHORE_DISABLED, MAXLONG, (wshMemName + L"_m_rsem").c_str());
    m_slave_received = CreateSemaphoreW(NULL, SEMAPHORE_DISABLED, MAXLONG, (wshMemName + L"_s_rsem").c_str());
    m_master_sent = CreateSemaphoreW(NULL, SEMAPHORE_DISABLED, MAXLONG, (wshMemName + L"_m_sent").c_str());
    m_slave_sent = CreateSemaphoreW(NULL, SEMAPHORE_DISABLED, MAXLONG, (wshMemName + L"_s_sent").c_str());
    m_slave_ready = CreateSemaphoreW(NULL, SEMAPHORE_ENABLED, MAXLONG, (wshMemName + L"_s_ready").c_str());

    if (m_master_received == NULL || m_slave_received == NULL ||
        m_master_sent == NULL || m_slave_sent == NULL || m_slave_ready == NULL)
    {
        perror("ProcCommunicator sem_open failure.");
        exit(1);
    }
#endif
}
