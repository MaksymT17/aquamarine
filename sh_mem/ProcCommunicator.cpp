#include "ProcCommunicator.h"
#include <iostream>
#include <unistd.h>

static constexpr int SEMAPHORE_DISABLED = 0;
static constexpr int SEMAPHORE_ENABLED = 1;
static constexpr const char *M_SEM_RECEIVED = "/master_rsem";
static constexpr const char *S_SEM_RECEIVED = "/slave_rsem";
static constexpr const char *M_SEM_SENT = "/master_sent";
static constexpr const char *S_SEM_SENT = "/slave_sent";
static constexpr const char *S_READY = "/slave_ready";

ProcCommunicator::ProcCommunicator(const bool isMasterMode,
                                   const bool isMultipleMasters,
                                   const std::string &shMemName) : m_master_mode(isMasterMode),
                                                                   m_multiple_master(isMultipleMasters),
                                                                   m_master_received((sem_t *)-1),
                                                                   m_slave_received((sem_t *)-1),
                                                                   m_master_sent((sem_t *)-1),
                                                                   m_slave_sent((sem_t *)-1)
{
    const std::string master_mem_name = shMemName + "_master";
    const std::string slave_mem_name = shMemName + "_slave";
    if (isMasterMode)
    {
        m_sender = std::make_unique<SharedMemorySender>(master_mem_name.c_str());
        m_receiver = std::make_unique<SharedMemoryReceiver>(slave_mem_name.c_str());

        m_master_received = sem_open(M_SEM_RECEIVED, O_CREAT, 0666, SEMAPHORE_DISABLED);
        m_slave_received = sem_open(S_SEM_RECEIVED, O_CREAT, 0666, SEMAPHORE_DISABLED);
        m_master_sent = sem_open(M_SEM_SENT, O_CREAT, 0666, SEMAPHORE_DISABLED);
        m_slave_sent = sem_open(S_SEM_SENT, O_CREAT, 0666, SEMAPHORE_DISABLED);

        if (isMultipleMasters)
            m_slave_ready = sem_open(S_READY, O_CREAT, 0666, SEMAPHORE_ENABLED);

        if (m_master_received == SEM_FAILED || m_slave_received == SEM_FAILED ||
            m_master_sent == SEM_FAILED || m_slave_sent == SEM_FAILED || m_slave_ready == SEM_FAILED)
        {
            perror("ProcCommunicator sem_open failure");
        }
    }
    else
    {
        m_sender = std::make_unique<SharedMemorySender>(slave_mem_name.c_str());
        m_receiver = std::make_unique<SharedMemoryReceiver>(master_mem_name.c_str());

        while (m_master_received == SEM_FAILED || m_slave_received == SEM_FAILED ||
               m_master_sent == SEM_FAILED || m_slave_sent == SEM_FAILED || m_slave_ready == SEM_FAILED)
        {
            m_master_received = sem_open(M_SEM_RECEIVED, O_RDWR, 0666, SEMAPHORE_DISABLED);
            m_slave_received = sem_open(S_SEM_RECEIVED, O_RDWR, 0666, SEMAPHORE_DISABLED);
            m_master_sent = sem_open(M_SEM_SENT, O_RDWR, 0666, SEMAPHORE_DISABLED);
            m_slave_sent = sem_open(S_SEM_SENT, O_RDWR, 0666, SEMAPHORE_DISABLED);

            if (isMultipleMasters)
                m_slave_ready = sem_open(S_READY, O_RDWR, 0666, SEMAPHORE_ENABLED);
        }
    }
}
ProcCommunicator::~ProcCommunicator()
{
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
}

void ProcCommunicator::send(const Message *msg)
{
    if (m_multiple_master && m_master_mode)
        sem_wait(m_slave_ready);

    m_sender->sendMessage(msg);
    sem_post(m_master_mode ? m_master_sent : m_slave_sent);
    sem_wait(m_master_mode ? m_slave_received : m_master_received);

    if (m_multiple_master && !m_master_mode)
        sem_post(m_slave_ready);
}

Message *ProcCommunicator::receive()
{
    sem_wait(m_master_mode ? m_slave_sent : m_master_sent);
    Message *response = m_receiver->receiveMessage();
    sem_post(m_master_mode ? m_master_received : m_slave_received);

    return response;
}

Message *ProcCommunicator::sendAndGetResponse(const Message *msg)
{
    m_sender->sendMessage(msg);
    return m_receiver->receiveMessage();
}
