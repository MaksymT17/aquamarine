#pragma once
#include <semaphore.h>
#include "Message.hpp"

class SharedMemorySender
{
public:
    SharedMemorySender(const char *shMemName);
    void init();
    void finish();
    void sendMessage(const Message *msg);

private:
    int m_shm_fd;
    void *m_ptr;
    sem_t *m_sem;
    sem_t *m_rec_sem;
    std::string m_name;
};