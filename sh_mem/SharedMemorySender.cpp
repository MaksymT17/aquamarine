#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <exception>
#include "SharedMemorySender.h"

const int SHARED_MEMORY_SIZE = 4096; // 4KB

SharedMemorySender::SharedMemorySender(const char *shMemName) : m_name(shMemName)
{
    init();
}
void SharedMemorySender::init()
{
    // Try to create the shared memory segment
    m_shm_fd = shm_open(m_name.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
    if (m_shm_fd == -1)
    {
        if (errno == EEXIST)
        {
            // Segment already exists, open it without O_CREAT | O_EXCL
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

    // Map the shared memory object into the address space of the process
    m_ptr = mmap(0, SHARED_MEMORY_SIZE, PROT_WRITE, MAP_SHARED, m_shm_fd, 0);
    if (m_ptr == MAP_FAILED)
    {
        std::cerr << "mmap failed" << std::endl;
        throw std::exception();
    }
}

void SharedMemorySender::finish()
{
    if (munmap(m_ptr, SHARED_MEMORY_SIZE) == -1)
    {
        std::cerr << "munmap failed" << std::endl;
    }
    if (close(m_shm_fd) == -1)
    {
        std::cerr << "close failed" << std::endl;
    }

    if (shm_unlink(m_name.c_str()) == -1)
    {
        std::cerr << "shm_unlink failed" << std::endl;
    }
}

void SharedMemorySender::sendMessage(const Message *msg)
{
    //    std::cout<<"sendMessage\n";
    if (msg->type == MessageType::SET_CONFIG)
        std::memcpy(m_ptr, msg, sizeof(MessageSetConfig));
    else if (msg->type == MessageType::COMPARE_REQUEST)
        std::memcpy(m_ptr, msg, sizeof(MessageCompareRequest));
    else if (msg->type == MessageType::COMPARE_RESULT)
        std::memcpy(m_ptr, msg, sizeof(MessageCompareResult));
    else
        std::memcpy(m_ptr, msg, sizeof(Message));
}