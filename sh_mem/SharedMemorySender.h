#pragma once

#include <string>
#include "Message.hpp"
#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#pragma comment(lib, "user32.lib")
#endif

class SharedMemorySender
{
public:
    SharedMemorySender(const char *shMemName);
    void init();
    void finish();
    void sendMessage(const Message *msg);

private:
#ifndef _WIN32
    int m_shm_fd;
#else
    HANDLE m_shm_fd;
#endif
    void *m_ptr;
    std::string m_name;
};