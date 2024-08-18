#include "ProcCommunicator.h"
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#endif

static constexpr int SEMAPHORE_DISABLED = 0;
static constexpr int SEMAPHORE_ENABLED = 1;

ProcCommunicator::ProcCommunicator(const bool isMasterMode,
                                   const bool isMultipleMasters,
                                   const std::string &shMemName) : m_master_mode(isMasterMode),
                                                                   m_multiple_master(isMultipleMasters)/*,
                                                                   m_master_received((sem_t *)-1),
                                                                   m_slave_received((sem_t *)-1),
                                                                   m_master_sent((sem_t *)-1),
                                                                   m_slave_sent((sem_t *)-1)*/
{
    const std::string master_mem_name = shMemName + "_master";
    const std::string slave_mem_name = shMemName + "_slave";
#ifndef _WIN32
    if (isMasterMode)
    {
        m_sender = std::make_unique<SharedMemorySender>(master_mem_name.c_str());
        m_receiver = std::make_unique<SharedMemoryReceiver>(slave_mem_name.c_str());

        m_master_received = sem_open((shMemName + "_m_rsem").c_str(), O_RDWR, 0666, SEMAPHORE_DISABLED);
        m_slave_received = sem_open((shMemName + "_s_rsem").c_str(), O_RDWR, 0666, SEMAPHORE_DISABLED);
        m_master_sent = sem_open((shMemName + "_m_sent").c_str(), O_RDWR, 0666, SEMAPHORE_DISABLED);
        m_slave_sent = sem_open((shMemName + "_s_sent").c_str(), O_RDWR, 0666, SEMAPHORE_DISABLED);

        if (isMultipleMasters)
            m_slave_ready = sem_open((shMemName + "_s_ready").c_str(), O_RDWR, 0666, SEMAPHORE_ENABLED);
    }
    else
    {
        m_sender = std::make_unique<SharedMemorySender>(slave_mem_name.c_str());
        m_receiver = std::make_unique<SharedMemoryReceiver>(master_mem_name.c_str());

        m_master_received = sem_open((shMemName + "_m_rsem").c_str(), O_CREAT, 0666, SEMAPHORE_DISABLED);
        m_slave_received = sem_open((shMemName + "_s_rsem").c_str(), O_CREAT, 0666, SEMAPHORE_DISABLED);
        m_master_sent = sem_open((shMemName + "_m_sent").c_str(), O_CREAT, 0666, SEMAPHORE_DISABLED);
        m_slave_sent = sem_open((shMemName + "_s_sent").c_str(), O_CREAT, 0666, SEMAPHORE_DISABLED);

        if (isMultipleMasters)
            m_slave_ready = sem_open((shMemName + "_s_ready").c_str(), O_CREAT, 0666, SEMAPHORE_ENABLED);
    }
    if (m_master_received == SEM_FAILED || m_slave_received == SEM_FAILED ||
        m_master_sent == SEM_FAILED || m_slave_sent == SEM_FAILED || m_slave_ready == SEM_FAILED)
    {
        perror("ProcCommunicator sem_open failure.");
        exit(1);
    }
    if(isMultipleMasters && m_slave_ready==SEM_FAILED){
        perror("ProcCommunicator MultiMaster mode sem_open failure.");
        exit(1);
    }
#else
    m_sender = std::make_unique<SharedMemorySender>(slave_mem_name.c_str());
    m_receiver = std::make_unique<SharedMemoryReceiver>(master_mem_name.c_str());

    std::wstring wshMemName(shMemName.begin(), shMemName.end());

    if (!(m_master_received = CreateSemaphoreW(NULL, SEMAPHORE_DISABLED, MAXLONG, (wshMemName + L"_m_rsem").c_str())))
        m_master_received = OpenSemaphoreW(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, 0, (wshMemName + L"_m_rsem").c_str());

    if (!(m_slave_received = CreateSemaphoreW(NULL, SEMAPHORE_DISABLED, MAXLONG, (wshMemName + L"_s_rsem").c_str())))
        m_slave_received = OpenSemaphoreW(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, 0, (wshMemName + L"_s_rsem").c_str());

    if (!(m_master_sent = CreateSemaphoreW(NULL, SEMAPHORE_DISABLED, MAXLONG, (wshMemName + L"_m_sent").c_str())))
        m_master_sent = OpenSemaphoreW(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, 0, (wshMemName + L"_m_sent").c_str());

    if (!(m_slave_sent = CreateSemaphoreW(NULL, SEMAPHORE_DISABLED, MAXLONG, (wshMemName + L"_s_ready").c_str())))
        m_slave_sent = OpenSemaphoreW(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, 0, (wshMemName + L"_s_ready").c_str());

    if (isMultipleMasters)
    {
        if (!(m_slave_ready = CreateSemaphoreW(NULL, SEMAPHORE_DISABLED, MAXLONG, (wshMemName + L"_s_ready").c_str())))
            m_slave_ready = OpenSemaphoreW(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, 0, (wshMemName + L"_s_ready").c_str());
    }

    if (m_master_received == NULL || m_slave_received == NULL ||
        m_master_sent == NULL || m_slave_sent == NULL || m_slave_ready == NULL )
    {
        perror("ProcCommunicator sem_open failure.");
        exit(1);
    }

#endif
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
    if (m_multiple_master && sem_close(m_slave_ready) == -1)
    {
        perror("Failed to destroy m_slave_ready semaphore");
    }
#else
    if (m_master_received && CloseHandle(m_master_received))
    {
        perror("Failed to destroy m_master_received semaphore");
    }
    if (m_slave_received && CloseHandle(m_slave_received))
    {
        perror("Failed to destroy m_slave_received semaphore");
    }
    if (m_master_sent && CloseHandle(m_master_sent))
    {
        perror("Failed to destroy m_master_sent semaphore");
    }
    if (m_slave_sent && CloseHandle(m_slave_sent))
    {
        perror("Failed to destroy m_slave_sent semaphore");
    }
    if (m_multiple_master && m_slave_ready && CloseHandle(m_slave_ready))
    {
        perror("Failed to destroy m_slave_ready semaphore");
    }
#endif
}
#ifndef _WIN32
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
#else
void ProcCommunicator::send(const Message *msg)
{
    // Wait for the semaphore to be signaled
    if (m_multiple_master && m_master_mode) {
        WaitForSingleObject(m_slave_ready, INFINITE); // INFINITE timeout to wait indefinitely
    }

    // Send the message
    m_sender->sendMessage(msg);

    // Post (release) the semaphore
    ReleaseSemaphore(m_master_mode ? m_master_sent : m_slave_sent, 1, NULL);

    // Wait for the semaphore to be signaled
    WaitForSingleObject(m_master_mode ? m_slave_received : m_master_received, INFINITE);

    // Post (release) the semaphore if needed
    if (m_multiple_master && !m_master_mode) {
        ReleaseSemaphore(m_slave_ready, 1, NULL);
    }
} 

Message *ProcCommunicator::receive()
{
    DWORD waitResult = WaitForSingleObject(m_master_mode ? m_slave_sent : m_master_sent, INFINITE);
    if (waitResult != WAIT_OBJECT_0) {
        // Handle error
        return nullptr;
    }

    // Receive the message
    Message *response = m_receiver->receiveMessage();

    // Post (release) the semaphore
    BOOL releaseResult = ReleaseSemaphore(m_master_mode ? m_master_received : m_slave_received, 1, NULL);
    if (!releaseResult) {
        // Handle error
        return nullptr;
    }

    return response;
}
#endif

Message *ProcCommunicator::sendAndGetResponse(const Message *msg)
{
    m_sender->sendMessage(msg);
    return m_receiver->receiveMessage();
}
