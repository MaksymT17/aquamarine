#pragma once
#include <string>
#include "Message.hpp"

class SharedMemoryReceiver
{
public:
    SharedMemoryReceiver(const char *shMemName);
    void init();
    void finish();
    Message *receiveMessage();

private:
#ifndef _WIN32
    int m_shm_fd;
#else
    HANDLE m_shm_fd;
#endif

    void *m_ptr;
    std::string m_name;
};