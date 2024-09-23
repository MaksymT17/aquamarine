#include <iostream>
#include <cstdlib>
#include <cstring>
#ifndef _WIN32
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#else
#include <windows.h>
#include <string>
#include <iostream>
#endif
#include "SharedMemoryReceiver.h"
#include <vector>

const int SHARED_MEMORY_SIZE = 4096; // 4KB

SharedMemoryReceiver::SharedMemoryReceiver(const char *shMemName) : m_name(shMemName)
{
    init();
}
#ifndef _WIN32
void SharedMemoryReceiver::init()
{
    m_shm_fd = shm_open(m_name.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
    if (m_shm_fd == -1)
    {
        if (errno == EEXIST)
        {
            m_shm_fd = shm_open(m_name.c_str(), O_RDWR, 0666);
            if (m_shm_fd == -1)
            {
                perror("shm_open failed");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            perror("shm_open failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Segment created successfully, truncate it to the desired size
        if (ftruncate(m_shm_fd, SHARED_MEMORY_SIZE) == -1)
        {
            perror("ftruncate failed");
            exit(EXIT_FAILURE);
        }
    }

    m_ptr = mmap(0, SHARED_MEMORY_SIZE, PROT_READ, MAP_SHARED, m_shm_fd, 0);
    if (m_ptr == MAP_FAILED)
    {
        perror("mmap failed");
        return;
    }
}
void SharedMemoryReceiver::finish()
{
    // Unmap and close the shared memory object
    if (munmap(m_ptr, SHARED_MEMORY_SIZE) == -1)
    {
        std::cerr << "munmap failed" << std::endl;
    }
    if (close(m_shm_fd) == -1)
    {
        std::cerr << "close failed" << std::endl;
    }
}
#else
void SharedMemoryReceiver::init()
{
    std::wstring wshMemName(m_name.begin(), m_name.end());
    m_shm_fd = OpenFileMappingW(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        wshMemName.c_str());

    if (m_shm_fd == NULL)
    {
        m_shm_fd = CreateFileMappingW(
            INVALID_HANDLE_VALUE, // use paging file
            NULL,                 // default security
            PAGE_READWRITE,       // read/write access
            0,                    // maximum object size (high-order DWORD)
            SHARED_MEMORY_SIZE,   // maximum object size (low-order DWORD)
            wshMemName.c_str());  // name of mapping object

        if (m_shm_fd == NULL)
        {
            printf(("Could not open file mapping object (%d).\n"),
                   GetLastError());
            return;
        }
    }

    m_ptr = (void *)MapViewOfFile(m_shm_fd,            // handle to map object
                                  FILE_MAP_ALL_ACCESS, // read/write permission
                                  0,
                                  0,
                                  SHARED_MEMORY_SIZE);

    if (m_ptr == NULL)
    {
        printf("Could not map view of file (%d).\n", GetLastError());

        CloseHandle(m_shm_fd);

        return;
    }
}

void SharedMemoryReceiver::finish()
{
    UnmapViewOfFile(m_ptr);
    CloseHandle(m_shm_fd);
}
#endif

Message *SharedMemoryReceiver::receiveMessage()
{
    Message *message(static_cast<Message *>(m_ptr));
    return message;
}