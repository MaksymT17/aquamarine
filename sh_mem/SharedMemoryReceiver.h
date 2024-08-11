#pragma once
#include <string>
#include "Message.hpp"

class SharedMemoryReceiver
{
public:
    SharedMemoryReceiver(const char *shMemName);
    ~SharedMemoryReceiver();

#ifdef WIN32
    void initWindowsSharedMemory();
    void finishWindows();
#else
    void initUnixSharedMemory();
    void finish();
#endif
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