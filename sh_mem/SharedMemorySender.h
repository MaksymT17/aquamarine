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
    ~SharedMemorySender();
#ifdef WIN32
    void initWindows();
    void finishWindows();
#else
    void init();
    void finish();
#endif
    void sendMessage(const Message *msg);

private:
void *m_ptr;
#ifndef _WIN32
    int m_shm_fd;
    
#else
    HANDLE m_shm_fd;
#endif
    std::string m_name;
};